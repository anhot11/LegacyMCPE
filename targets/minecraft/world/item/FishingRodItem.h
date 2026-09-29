#pragma once

#include "Item.h"

class Player;
class Level;
class Icon;

class FishingRodItem : public Item {
private:
    Icon* emptyIcon;

public:
    FishingRodItem(int id);

    virtual bool isHandEquipped();
    virtual bool isMirroredArt();
    virtual std::shared_ptr<ItemInstance> use(
        std::shared_ptr<ItemInstance> instance, Level* level,
        std::shared_ptr<Player> player);

    void registerIcons(IconRegister* iconRegister);
    Icon* getEmptyIcon();
};
