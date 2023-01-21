
#include "board.h"

#include <iostream>
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

void Board::moveAll() {
    for (auto& entity : entities) {
        entity->moveInBoundries({cols, rows});
        getEntitiesInFov(entity);
    }
}

// TODO: return something
void Board::getEntitiesInFov(EntityPtr entity) {
    Radian fovPart = entity->fieldOfView() / 2;
    Radian fovStart = entity->angle() - fovPart;
    Radian fovEnd = entity->angle() + fovPart;

    struct entDist {
        EntityPtr entity;
        distance_t distance;
    };
    std::unordered_map<Radian, entDist, Radian::Hasher> inSight;

    for (const auto& otherEntity: entities) {
        distance_t dist = getDistance(entity->location(), otherEntity->location());
        if (dist > entity->maxSightDistance()) {
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
        if (sawEntity == inSight.end() || dist < sawEntity->second.distance) {
            inSight[angle] = {otherEntity, dist};
        }
    }

    std::cout << "I'm entity " << entity->toString() << std::endl;
    for (const auto& [angle, entityDist] : inSight) {
        std::cout << "--> I see " << entityDist.entity->toString() << " at distance " << entityDist.distance << std::endl;
    }
    std::cout << std::endl;

}