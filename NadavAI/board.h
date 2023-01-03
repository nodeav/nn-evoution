#pragma once

#include <vector>
#include <memory>
#include "entities.h"

using std::shared_ptr;

typedef shared_ptr<Entity> EntityPtr;
typedef std::vector<EntityPtr> entityContainer;

class Board {
    entityContainer entities;

    uint32_t rows;
    uint32_t cols;

public:
    Board(uint32_t row, uint32_t col);
    void addEntity(EntityPtr entity);
    void print() const;
    void moveAllEntities();
};