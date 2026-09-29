#pragma once

#include <memory>

#include "AbstractContainerMenu.h"

class CraftingContainer;
class Container;
class Inventory;
class Level;

class CraftingMenu : public AbstractContainerMenu {
    // 4J Stu Made these public for UI menus, perhaps should make friend class?
public:
    static const int RESULT_SLOT;
    static const int CRAFT_SLOT_START;
    static const int CRAFT_SLOT_END;
    static const int INV_SLOT_START;
    static const int INV_SLOT_END;
    static const int USE_ROW_SLOT_START;
    static const int USE_ROW_SLOT_END;

public:
    std::shared_ptr<CraftingContainer> craftSlots;
    std::shared_ptr<Container> resultSlots;

private:
    Level* level;
    int x, y, z;

public:
    CraftingMenu(std::shared_ptr<Inventory> inventory, Level* level, int xt,
                 int yt, int zt);

    virtual void
    slotsChanged();  // 4J used to take a std::shared_ptr<Container> but wasn't
                     // using it, so removed to simplify things
    virtual void removed(std::shared_ptr<Player> player);
    virtual bool stillValid(std::shared_ptr<Player> player);
    virtual std::shared_ptr<ItemInstance> quickMoveStack(
        std::shared_ptr<Player> player, int slotIndex);
    virtual bool canTakeItemForPickAll(std::shared_ptr<ItemInstance> carried,
                                       Slot* target);

    int getX() { return x; }
    int getY() { return y; }
    int getZ() { return z; }
};