#include "PoweredMetalTile.h"

#include "minecraft/world/level/redstone/Redstone.h"
#include "minecraft/world/level/tile/MetalTile.h"

PoweredMetalTile::PoweredMetalTile(int id) : MetalTile(id) {}

bool PoweredMetalTile::isSignalSource() { return true; }

int PoweredMetalTile::getSignal(LevelSource* level, int x, int y, int z,
                                int dir) {
    return Redstone::SIGNAL_MAX;
}