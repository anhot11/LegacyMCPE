#pragma once
#include <cstdint>

class EntityEvent {
public:
    static const std::uint8_t JUMP = 1;
    static const std::uint8_t HURT = 2;
    static const std::uint8_t DEATH = 3;
    static const std::uint8_t START_ATTACKING = 4;
    static const std::uint8_t STOP_ATTACKING = 5;

    static const std::uint8_t TAMING_FAILED = 6;
    static const std::uint8_t TAMING_SUCCEEDED = 7;
    static const std::uint8_t SHAKE_WETNESS = 8;

    static const std::uint8_t USE_ITEM_COMPLETE = 9;

    static const std::uint8_t EAT_GRASS = 10;
    static const std::uint8_t OFFER_FLOWER = 11;
    static const std::uint8_t LOVE_HEARTS = 12;
    static const std::uint8_t VILLAGER_ANGRY = 13;
    static const std::uint8_t VILLAGER_HAPPY = 14;
    static const std::uint8_t WITCH_HAT_MAGIC = 15;
    static const std::uint8_t ZOMBIE_CONVERTING = 16;

    static const std::uint8_t FIREWORKS_EXPLODE = 17;

    static const std::uint8_t IN_LOVE_HEARTS = 18;
};
