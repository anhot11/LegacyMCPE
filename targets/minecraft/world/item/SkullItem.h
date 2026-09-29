#pragma once

#include <memory>
#include <string>

#include "Item.h"

class Icon;
class ItemInstance;
class Level;
class Player;

class SkullItem : public Item {
private:
    static const int SKULL_COUNT = 5;

    static const unsigned int NAMES[SKULL_COUNT];

public:
    static std::string ICON_NAMES[SKULL_COUNT];

private:
    Icon* icons[SKULL_COUNT];

public:
    SkullItem(int id);

    bool useOn(std::shared_ptr<ItemInstance> instance,
               std::shared_ptr<Player> player, Level* level, int x, int y,
               int z, int face, float clickX, float clickY, float clickZ,
               bool bTestUseOnOnly = false);
    bool mayPlace(Level* level, int x, int y, int z, int face,
                  std::shared_ptr<Player> player,
                  std::shared_ptr<ItemInstance> item);
    Icon* getIcon(int itemAuxValue);
    int getLevelDataForAuxValue(int auxValue);
    virtual unsigned int getDescriptionId(int iData = -1);
    unsigned int getDescriptionId(std::shared_ptr<ItemInstance> instance);
    std::string getHoverName(std::shared_ptr<ItemInstance> itemInstance);
    void registerIcons(IconRegister* iconRegister);
};