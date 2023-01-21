
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
    }
}

// TODO: return something
void Board::getEntitiesInFov(EntityPtr entity) {
    Radian fovPart = entity->fieldOfView() / 2;
    Radian fovBelow = entity->angle() - fovPart;
    Radian fovAbove = entity->angle() + fovPart;

    typedef std::pair<EntityPtr, distance_t> entDist;
    std::unordered_map<Radian, entDist, RadianHasher> inSight;

    for (const auto& otherEntity: entities) {
        distance_t dist = getDistance(entity->location(), otherEntity->location());
        if (dist > entity->maxSightDistance()) {
            continue;
        }

        Radian angle = getAngle(entity->location(), otherEntity->location());

        // as first step, only exact same angle can block the sight
        // in the next step, we can use the radius of the entity we see
        // in the next-next step, perspective can be implemented

        // TODO: handle overflowing the circle
        if (angle <= fovAbove && angle >= fovBelow) {
            auto sawEntity = inSight.find(angle);
            if (sawEntity == inSight.end()) {
                inSight.insert({angle, std::make_pair(otherEntity, dist)});
            } else {
                if (sawEntity->second.second > dist) {
                    inSight[angle] = std::make_pair(otherEntity, dist);
                }
            }

        }
    }
}