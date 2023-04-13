#pragma once

#include <vector>
#include <memory>
#include "entities.h"
#include "misc_util/misc_util.h"

typedef std::vector<EntityPtr> entityContainer;


class Board {
    entityContainer entities;
    std::unique_ptr<ThreadPool> threadPool;
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