
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
    float n = (loc.y() - m * (loc.x()));
    return n;
}

void Board::oneRayOfEntity(EntityPtr entity) {
    // 1) calculate m from angle
    slope_t m = slope(entity->angle());

    // 2) calcualte n from x, y, m -> we now have a line equation-> A
    float n = getN(entity->location(), m);


    // 3) calculate redbox from the line equation(A) + sight-range



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