#include "board.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include "utils.h"


Board::Board(float row, float col) {
    this->rows = row;
    this->cols = col;
    threadPool = std::make_unique<ThreadPool>(12);
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
            auto entitiesInFov = getEntitiesInFov(entity);
            entity->acknowledgeEntities(entitiesInFov);
        }));
    }

    for(const auto& future : futures) {
        future.wait();
    }
    futures.clear();

    for (auto& entity : entities) {
        futures.push_back(threadPool->enqueue([&]() {
            entity->moveInBoundries({cols, rows});
        }));
    }
    for(const auto& future : futures) {
        future.wait();
    }
}

std::vector<EntityDistanceResult> Board::getEntitiesInFov(const EntityPtr& entity) {
    Radian fovPart = entity->fieldOfView() / 2;
    Radian fovStart = entity->angle() - fovPart;
    Radian fovEnd = entity->angle() + fovPart;

    std::vector<EntityDistanceResult> ret;

    // TODO: hash with something vaguer than float
    std::unordered_map<int, EntityDistanceResult> inSight;

    for (const auto& otherEntity: entities) {
        if (entity == otherEntity) {
            continue;
        }

        distance_t distSquared = getDistanceSquared(entity->location(), otherEntity->location());
        auto maxDist = entity->maxSightDistance();
        if (distSquared > (maxDist * maxDist)) {
            continue;
        }

        Radian angle = getAngle(entity->location(), otherEntity->location());
        auto intAngle = angle.toIntDegrees();
        // as first step, only exact same angle can block the sight
        // in the next step, we can use the radius of the entity we see
        // in the next-next step, perspective can be implemented

        if (!angle.between(fovStart, fovEnd)) {
            continue;
        }

        auto sawEntity = inSight.find(intAngle);
        if (sawEntity == inSight.end() || distSquared < sawEntity->second.distanceToEntity) {
            inSight[intAngle] = {otherEntity->speed(), angle, distSquared};
        }
    }

    for (auto& [angle, entityDist] : inSight) {
        entityDist.distanceToEntity = sqrtf(entityDist.distanceToEntity);
        ret.emplace_back(entityDist);
    }
    return ret;
}