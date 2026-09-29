#pragma once

#include <stdint.h>

#include <string>

#include "Item.h"

class FireworksItem : public Item {
public:
    static const std::string TAG_FIREWORKS;
    static const std::string TAG_EXPLOSION;
    static const std::string TAG_EXPLOSIONS;
    static const std::string TAG_FLIGHT;
    static const std::string TAG_E_TYPE;
    static const std::string TAG_E_TRAIL;
    static const std::string TAG_E_FLICKER;
    static const std::string TAG_E_COLORS;
    static const std::string TAG_E_FADECOLORS;

    static const uint8_t TYPE_SMALL = 0;
    static const uint8_t TYPE_BIG = 1;
    static const uint8_t TYPE_STAR = 2;
    static const uint8_t TYPE_CREEPER = 3;
    static const uint8_t TYPE_BURST = 4;

    static const uint8_t TYPE_MIN = TYPE_SMALL;
    static const uint8_t TYPE_MAX = TYPE_BURST;

    FireworksItem(int id);

    bool useOn(std::shared_ptr<ItemInstance> instance,
               std::shared_ptr<Player> player, Level* level, int x, int y,
               int z, int face, float clickX, float clickY, float clickZ,
               bool bTestUseOnOnly = false);
    void appendHoverText(std::shared_ptr<ItemInstance> itemInstance,
                         std::shared_ptr<Player> player,
                         std::vector<HtmlString>* lines, bool advanced);
};