#include "board.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include "utils.h"

Board::Board(float row, float col) {
    this->rows = row;
    this->cols = col;
#ifdef CAT_GPT_REPRODUCIBLE
    auto num_threads = 1;
#else
    auto num_threads = 23;
#endif
    threadPool = std::make_unique<ThreadPool>(num_threads);
}

void Board::addEntity(EntityPtr entity) {
    entities.emplace_back(entity);
}

void Board::print() const {
    for (const auto &entity: entities) {
        std::cout << entity->toString() << std::endl;
    }
}

bool enablePrint = false;

// todo: prettify
void Board::moveAll() {
    std::vector<std::future<void>> futures;
    futures.reserve(entities.size());

    for (auto& entity : entities) {
        futures.push_back(threadPool->enqueue([&]() {
            std::vector<EntityDistanceResult> entitiesInFov = getEntitiesInFov(entity, entityNum);
            entity->maybeEat(entitiesInFov);
            entity->acknowledgeEntities(entitiesInFov);
        }));
    }

    for(const auto& future : futures) {
        future.wait();
    }
    futures.clear();

    std::vector<EntityPtr> newEntities;
    std::mutex mut;

    for (auto& entity : entities) {
        futures.push_back(threadPool->enqueue([&]() {
            entity->moveInBoundries({cols, rows});
            EntityPtr possibleEntity = entity->maybeGiveBirth();
            if (possibleEntity) {
                switch (entity->getType()) {
                    case EntityType::TARIF:
                        this->tarifs_born++;
                        break;
                    case EntityType::TOREF:
                        this->torefs_born++;
                }
                std::lock_guard<std::mutex> guard(mut);
                newEntities.push_back(possibleEntity);
            }
        }));
    }

    for(const auto& future : futures) {
        future.wait();
    }

    entities.erase(
        std::remove_if(entities.begin(), entities.end(), [&] (auto ent) {
            if (!ent->isDead()) {
                return false;
            }
            switch (ent->getType()) {
                case EntityType::TARIF:
                    this->tarifs_eaten++;
                    break;
                case EntityType::TOREF:
                    this->torefs_died++;
            }
            return true;
        }), entities.end()
    );

    entities.insert(entities.end(), newEntities.begin(), newEntities.end());
    std::cout << "[gen " << ++gen << "] torefs died: " << torefs_died << ", torefs born: " << torefs_born <<
        ", tarifs eaten: " << tarifs_eaten << ", tarifs born: " << tarifs_born << std::endl;
}

std::vector<EntityDistanceResult> Board::getEntitiesInFov(const EntityPtr &entity, const int numEntities) {
    Radian fovPart = entity->fieldOfView() / 2;
    Radian fovStart = entity->angle() - fovPart;
    Radian fovEnd = entity->angle() + fovPart;

    std::vector<EntityDistanceResult> ret;

    // TODO: hash with something vaguer than float
    std::unordered_map<int, EntityDistanceResult> inSight;

    auto maxDist = entity->maxSightDistance();
    auto maxDistSquared = maxDist * maxDist;
    // just another filter; it's very much imperfect, but can filter out some of the entities
    distance_t largestDistanceSquared = 0;
    auto entityLocation = entity->location();
    for (const auto &otherEntity: entities) {
//        if (entity == otherEntity) {
//            continue;
//        }

        distance_t distSquared = getDistanceSquared(entityLocation, otherEntity->location());
        if (distSquared > maxDistSquared) {
            continue;
        }

        // TODO: is this worth it?
        // don't proceed if we have enough entities and this one is further away than others
        if (distSquared > maxDistSquared && inSight.size() >= numEntities) {
            continue;
        }

        Radian angle = getAngle(entityLocation, otherEntity->location());
        auto intAngle = angle.toIntDegrees();

        if (!angle.between(fovStart, fovEnd)) {
            continue;
        }

        auto sawEntity = inSight.find(intAngle);
        if (sawEntity == inSight.end() || distSquared < sawEntity->second.distanceToEntity) {
            largestDistanceSquared = std::max(largestDistanceSquared, distSquared);
            inSight[intAngle] = {otherEntity, angle, distSquared};
        }
    }

    for (auto& [angle, entityDist] : inSight) {
        entityDist.distanceToEntity = sqrtf(entityDist.distanceToEntity);
        ret.emplace_back(entityDist);
    }
    return ret;
}