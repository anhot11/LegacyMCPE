#pragma once

#include <memory>

#include "Slot.h"

class Container;
class Player;

class FurnaceResultSlot : public Slot {
private:
    std::shared_ptr<Player> player;
    int removeCount;

public:
    FurnaceResultSlot(std::shared_ptr<Player> player,
                      std::shared_ptr<Container> container, int slot, int x,
                      int y);
    virtual ~FurnaceResultSlot() {}

    virtual bool mayPlace(std::shared_ptr<ItemInstance> item);
    virtual std::shared_ptr<ItemInstance> remove(int c);
    virtual void onTake(std::shared_ptr<Player> player,
                        std::shared_ptr<ItemInstance> carried);
    virtual bool mayCombine(std::shared_ptr<ItemInstance> item);  // 4J Added

protected:
    virtual void onQuickCraft(std::shared_ptr<ItemInstance> picked, int count);
    virtual void checkTakeAchievements(std::shared_ptr<ItemInstance> carried);
};