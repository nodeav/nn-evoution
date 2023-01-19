
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