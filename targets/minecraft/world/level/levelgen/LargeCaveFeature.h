#pragma once

#include <stdint.h>

#include <format>
#include <vector>

#include "LargeFeature.h"
#include "minecraft/world/level/levelgen/LargeFeature.h"

class LargeCaveFeature : public LargeFeature {
protected:
    void addRoom(int64_t seed, int xOffs, int zOffs,
                 std::vector<uint8_t>& blocks, double xRoom, double yRoom,
                 double zRoom);
    void addTunnel(int64_t seed, int xOffs, int zOffs,
                   std::vector<uint8_t>& blocks, double xCave, double yCave,
                   double zCave, float thickness, float yRot, float xRot,
                   int step, int dist, double yScale);
    virtual void addFeature(Level* level, int x, int z, int xOffs, int zOffs,
                            std::vector<uint8_t>& blocks);
};
