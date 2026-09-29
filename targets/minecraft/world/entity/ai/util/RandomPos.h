#pragma once

#include <memory>
#include <optional>

#include "minecraft/world/phys/Vec3.h"

class PathfinderMob;

class RandomPos {
private:
    static Vec3 tempDir;

public:
    static std::optional<Vec3> getPos(std::shared_ptr<PathfinderMob> mob,
                                      int xzDist, int yDist,
                                      int quadrant = -1);  // 4J added quadrant
    static std::optional<Vec3> getPosTowards(std::shared_ptr<PathfinderMob> mob,
                                             int xzDist, int yDist,
                                             Vec3* towardsPos);
    static std::optional<Vec3> getPosAvoid(std::shared_ptr<PathfinderMob> mob,
                                           int xzDist, int yDist,
                                           Vec3* avoidPos);

private:
    static std::optional<Vec3> generateRandomPos(
        std::shared_ptr<PathfinderMob> mob, int xzDist, int yDist, Vec3* dir,
        int quadrant = -1);  // 4J added quadrant
};
