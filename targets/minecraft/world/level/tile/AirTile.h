#pragma once
#include "Tile.h"
#include "minecraft/world/level/material/Material.h"

class AirTile : public Tile {
    friend class Tile;

protected:
    AirTile(int id);
};