
#include "board.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include "utils.h"


Board::Board(float row, float col) {
    this->rows = row;
    this->cols = col;
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

void Board::moveAll() {
    for (auto& entity : entities) {
        auto entitiesInFov = getEntitiesInFov(entity);
        entity->acknowledgeEntities(entitiesInFov);
    }

    for (auto& entity : entities) {
        entity->moveInBoundries({cols, rows});
    }
}

std::vector<EntityDistanceResult> Board::getEntitiesInFov(const EntityPtr& entity) {
    Radian fovPart = entity->fieldOfView() / 2;
    Radian fovStart = entity->angle() - fovPart;
    Radian fovEnd = entity->angle() + fovPart;

    std::vector<EntityDistanceResult> ret;

    // TODO: hash with something vaguer than float
    std::unordered_map<Radian, EntityDistanceResult, Radian::Hasher> inSight;

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

        // as first step, only exact same angle can block the sight
        // in the next step, we can use the radius of the entity we see
        // in the next-next step, perspective can be implemented

        if (!angle.between(fovStart, fovEnd)) {
            continue;
        }

        auto sawEntity = inSight.find(angle);
        if (sawEntity == inSight.end() || distSquared < sawEntity->second.distanceToEntity) {
            inSight[angle] = {otherEntity->speed(), angle, distSquared};
        }
    }

    for (auto& [angle, entityDist] : inSight) {
        entityDist.distanceToEntity = sqrtf(entityDist.distanceToEntity);
        ret.emplace_back(entityDist);
    }
    return ret;
}