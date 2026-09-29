#pragma once

#include <memory>

#include "Slot.h"

class AnvilMenu;
class Container;

class RepairResultSlot : public Slot {
private:
    AnvilMenu* m_menu;
    int xt, yt, zt;

public:
    RepairResultSlot(AnvilMenu* menu, int xt, int yt, int zt,
                     std::shared_ptr<Container> container, int slot, int x,
                     int y);

    bool mayPlace(std::shared_ptr<ItemInstance> item) override;
    bool mayPickup(std::shared_ptr<Player> player) override;
    void onTake(std::shared_ptr<Player> player,
                std::shared_ptr<ItemInstance> carried) override;
    virtual bool mayCombine(
        std::shared_ptr<ItemInstance> item) override;  // 4J Added
};