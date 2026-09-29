#pragma once

#include <stdint.h>

#include <memory>
#include <unordered_map>
#include <vector>

#include "minecraft/Pos.h"

class Random;
class Entity;
class ServerLevel;

class PortalForcer {
public:
    class PortalPosition : public Pos {
    public:
        int64_t lastUsed;

        PortalPosition(int x, int y, int z, int64_t time);
    };

private:
    ServerLevel* level;
    Random* random;
    std::unordered_map<int64_t, PortalPosition*> cachedPortals;
    std::vector<int64_t> cachedPortalKeys;

public:
    PortalForcer(ServerLevel* level);
    ~PortalForcer();

    void force(std::shared_ptr<Entity> e, double xOriginal, double yOriginal,
               double zOriginal, float yRotOriginal);
    bool findPortal(std::shared_ptr<Entity> e, double xOriginal,
                    double yOriginal, double zOriginal, float yRotOriginal);
    bool createPortal(std::shared_ptr<Entity> e);
    void tick(int64_t time);
};