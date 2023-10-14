#pragma once

#include <vector>
#include <memory>
#include <atomic>
#include "entities.h"
#include "misc_util/misc_util.h"

typedef std::vector<EntityPtr> entityContainer;


class Board {
    entityContainer entities;
    std::unique_ptr<ThreadPool> threadPool;
    float rows;
    float cols;
    std::atomic <int64_t> torefs_died{}, torefs_born{}, tarifs_eaten{}, tarifs_born{};

public:
    Board(float row, float col);
    void addEntity(EntityPtr entity);
    void print() const;
    void moveAll();
    entityContainer getEntities() const { return entities; };
    std::vector<EntityDistanceResult> getEntitiesInFov(const EntityPtr &entity, const int numEntities);
    uint64_t gen = 0;
};