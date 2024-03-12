#include "board.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include "utils.h"


Board::Board(float row, float col) {
    this->rows = row;
    this->cols = col;
    threadPool = std::make_unique<ThreadPool>(12);
}

void Board::addToref(TorefPtr toref) {
    torfim.emplace_back(toref);
}

void Board::addTarif(TarifPtr tarif) {
    tarifim.emplace_back(tarif);
}

// todo: prettify
void Board::moveAll() {
    std::vector<std::future<void>> futures;
    futures.reserve(tarifim.size() + torfim.size());

    // Note that everyone are moving and eating at the same time and there might be a race where
    //  two torefs are eating the same tarif
    for (auto& tarif : tarifim) {
        futures.push_back(threadPool->enqueue([&]() {
            std::vector<EntityDistanceResult> entitiesInFov = getEntitiesInFov(tarif, torfim);
            tarif->acknowledgeEntities(entitiesInFov);
        }));
    }

    for (auto& toref : torfim) {
        futures.push_back(threadPool->enqueue([&]() {
            std::vector<EntityDistanceResult> entitiesInFov = getEntitiesInFov(toref, tarifim);
            toref->maybeEat(entitiesInFov);
            toref->acknowledgeEntities(entitiesInFov);
        }));
    }

    for(const auto& future : futures) {
        future.wait();
    }
    futures.clear();

    std::vector<TorefPtr> newTorfim;
    std::vector<TarifPtr> newTarifim;
    std::mutex mut;

    for (auto& toref : torfim) {
        futures.push_back(threadPool->enqueue([&]() {
            toref->moveInBoundries({cols, rows});
            TorefPtr possibleToref = toref->maybeGiveBirth();
            if (possibleToref) {
                std::lock_guard<std::mutex> guard(mut);
                newTorfim.push_back(possibleToref);
            }
        }));
    }

    for (auto& tarif : tarifim) {
        futures.push_back(threadPool->enqueue([&]() {
            tarif->moveInBoundries({cols, rows});
            TarifPtr possibleTarif = tarif->maybeGiveBirth();
            if (possibleTarif) {
                std::lock_guard<std::mutex> guard(mut);
                newTarifim.push_back(possibleTarif);
            }
        }));
    }

    for(const auto& future : futures) {
        future.wait();
    }

    tarifim.erase(
        std::remove_if(tarifim.begin(), tarifim.end(), [] (auto ent) {
            return ent->isDead();
        }), tarifim.end()
    );

    torfim.erase(
        std::remove_if(torfim.begin(), torfim.end(), [] (auto ent) {
            return ent->isDead();
        }), torfim.end()
    );

    torfim.insert(torfim.end(), newTorfim.begin(), newTorfim.end());
    tarifim.insert(tarifim.end(), newTarifim.begin(), newTarifim.end());
    ++gen;
}

EntitiesContainer Board::getEntities() const {
    EntitiesContainer entities;
    entities.insert(entities.end(), tarifim.begin(), tarifim.end());
    entities.insert(entities.end(), torfim.begin(), torfim.end());
    return entities;
}

StatsPoint Board::getStats() const {
    return StatsPoint {
        .torefs = static_cast<uint32_t>(torfim.size()),
        .tarifs = static_cast<uint32_t>(tarifim.size()),
        .frame = static_cast<uint32_t>(gen)
    };
}
