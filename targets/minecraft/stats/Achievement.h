#pragma once

#include <memory>
#include <string>

#include "Stat.h"

class DescFormatter;
class Item;
class ItemInstance;
class Tile;

class Achievement : public Stat {
public:
    const int x, y;
    Achievement* prerequisite;

private:
    const std::string desc;
    DescFormatter* descFormatter;

public:
    const std::shared_ptr<ItemInstance> icon;

private:
    bool isGoldenVar;
    void _init();

public:
    Achievement(int id, const std::string& name, int x, int y, Item* icon,
                Achievement* prerequisite);
    Achievement(int id, const std::string& name, int x, int y, Tile* icon,
                Achievement* prerequisite);
    Achievement(int id, const std::string& name, int x, int y,
                std::shared_ptr<ItemInstance> icon, Achievement* prerequisite);

    Achievement* setAwardLocallyOnly();
    Achievement* setGolden();
    Achievement* postConstruct();
    bool isAchievement();
    std::string getDescription();
    Achievement* setDescFormatter(DescFormatter* descFormatter);
    bool isGolden();
    int getAchievementID();
};
