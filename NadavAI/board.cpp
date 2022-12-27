
#include "board.h"

#include <iostream>


Board::Board(uint32_t row, uint32_t col) {
    this->rows = row;
    this->cols = col;
}

void Board::AddEntity(EntityPtr entity) {
    entities.emplace_back(entity);
}

void Board::print() const {
    for (const auto entity: entities) {
        std::cout << entity->toString() << std::endl;
    }
}
