#pragma once

#include <vector>
#include <memory>
#include "entities.h"

using std::shared_ptr;

typedef shared_ptr<Entity> EntityPtr;
typedef std::vector<EntityPtr> entityContainer;

struct EntityDistanceResult {
    EntityPtr entity;
    distance_t distance{};
};

class Board {
    entityContainer entities;

    float rows;
    float cols;

public:
    Board(float row, float col);
    void addEntity(EntityPtr entity);
    void print() const;
    void moveAll();
    std::vector<EntityDistanceResult> getEntitiesInFov(const EntityPtr& entity);
};