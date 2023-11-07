#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include "entities.h"
#include "misc_util/misc_util.h"

typedef std::vector<EntityPtr> EntitiesContainer;
typedef std::vector<TorefPtr> TorefsContainer;
typedef std::vector<TarifPtr> TarifsContainer;


class Board {
    TorefsContainer torfim;
    TarifsContainer tarifim;
    std::unique_ptr<ThreadPool> threadPool;
    float rows;
    float cols;

public:
    Board(float row, float col);
    void addToref(TorefPtr toref);
    void addTarif(TarifPtr tarif);
    void moveAll();
    EntitiesContainer getEntities() const;

    uint64_t gen = 0;

    template <typename T, typename U>
    std::vector<EntityDistanceResult> getEntitiesInFov(const T& entity, std::vector<std::shared_ptr<U>> enemies) {
        Radian fovPart = entity->fieldOfView() / 2;
        Radian fovStart = entity->angle() - fovPart;
        Radian fovEnd = entity->angle() + fovPart;

        std::vector<EntityDistanceResult> ret;

        // TODO: hash with something vaguer than float
        std::unordered_map<int, EntityDistanceResult> inSight;

        for (const auto& otherEntity: enemies) {
            distance_t distSquared = getDistanceSquared(entity->location(), otherEntity->location());
            auto maxDist = entity->maxSightDistance();
            if (distSquared > (maxDist * maxDist)) {
                continue;
            }

            Radian angle = getAngle(entity->location(), otherEntity->location());
            auto intAngle = angle.toIntDegrees();

            if (!angle.between(fovStart, fovEnd)) {
                continue;
            }

            auto sawEntity = inSight.find(intAngle);
            if (sawEntity == inSight.end() || distSquared < sawEntity->second.distanceToEntity) {
                inSight[intAngle] = {otherEntity, angle, distSquared};
            }
        }

        for (auto& [angle, entityDist] : inSight) {
            entityDist.distanceToEntity = sqrtf(entityDist.distanceToEntity);
            ret.emplace_back(entityDist);
        }
        return ret;
    }
};