#pragma once
#include <memory>
#include <vector>

#include "minecraft/world/item/MapItem.h"

class Options;
class Font;
class Textures;
class Player;
class MapItemSavedData;

class Minimap {
private:
    static const int w = MapItem::IMAGE_WIDTH;
    static const int h = MapItem::IMAGE_HEIGHT;
    static int LUT[256];  // 4J added
    static bool genLUT;   // 4J added
    int renderCount;      // 4J added
    bool m_optimised;     // 4J Added
    std::vector<int> pixels;
    int mapTexture;
    Options* options;
    Font* font;

public:
    Minimap(Font* font, Options* options, Textures* textures,
            bool optimised = true);  // 4J Added optimised param
    static void reloadColours();
    void render(std::shared_ptr<Player> player, Textures* textures,
                std::shared_ptr<MapItemSavedData> data,
                int entityId);  // 4J added entityId param
};
