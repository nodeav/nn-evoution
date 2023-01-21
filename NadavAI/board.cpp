
#include "board.h"

#include <iostream>


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
    for (uint64_t i = 0; i < entities.size(); ++i) {
        entities[i]->moveInBoundries({cols, rows});
    }
}

typedef float slope_t;

static slope_t slope(Radian radian) {
    return tan(radian.toRadian());
}

static float getN(Location loc, slope_t m) {
    // y = mx + n
    // y - mx = n
    // n = y - mx
    return loc.y() - m * loc.x();
}

Radian getAngle(Location loc1, Location loc2) {
    if (loc1.x() == loc2.x()) {
        return loc1.y() > loc2.y() ? 0 : M_PI;
    }
    return std::atan((loc2.y() - loc1.y()) / (loc2.x() - loc1.x()));
}

void Board::getEntitiesInFov(EntityPtr entity) {
    Radian fovPart = entity->fieldOfView() / 2;
    Radian fov1 = entity->angle() + fovPart;
    Radian fov2 = entity->angle() - fovPart;

//    // 1) calculate m from angle
//    slope_t m = slope(otherEntity->angle());
//
//    // 2) calcualte n from x, y, m -> we now have a line equation-> A
//    float n = getN(otherEntity->location(), m);

    for (const auto& otherEntity : entities) {
        // TODO: check distance
        Radian angle = getAngle(entity->location(), otherEntity->location());

    }


    // 4) for each entity on the board
    //        only if inside redbox
    //            calculate m' from the (A) + 90degrees
    //            calculate the line equation(B) from x', y', m'
    //            find cut of A and B -> x'', y''.
    //            calculate distance between x', y' and (x'', y'')
    //            if (distance < entity.radius) {
    //                 entity is in sight
    //                 keep x'', y'' for later
    // 5) get the entity with shortest distance from x, y to x'', y''
}