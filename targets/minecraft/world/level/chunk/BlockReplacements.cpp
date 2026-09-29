#include "BlockReplacements.h"

#include <vector>

#include "minecraft/world/level/tile/Tile.h"

std::vector<uint8_t> BlockReplacements::replacements =
    std::vector<uint8_t>(256);

void BlockReplacements::staticCtor() {
    for (int i = 0; i < 256; i++) {
        uint8_t b = (uint8_t)i;
        if (b != 0 && Tile::tiles[b & 0xff] == nullptr) {
            b = 0;
        }
        BlockReplacements::replacements[i] = b;
    }
}

void BlockReplacements::replace(std::vector<uint8_t>& blocks) {
    for (unsigned int i = 0; i < blocks.size(); i++) {
        blocks[i] = replacements[blocks[i] & 0xff];
    }
}