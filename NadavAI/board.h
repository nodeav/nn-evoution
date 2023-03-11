#pragma once

#include <vector>
#include <memory>
#include "entities.h"

typedef std::vector<EntityPtr> entityContainer;


class Board {
    entityContainer entities;

    float rows;
    float cols;

public:
    Board(float row, float col);
    void addEntity(EntityPtr entity);
    void print() const;
    void moveAll();
    entityContainer getEntities() const { return entities; };
    std::vector<EntityDistanceResult> getEntitiesInFov(const EntityPtr& entity);
};