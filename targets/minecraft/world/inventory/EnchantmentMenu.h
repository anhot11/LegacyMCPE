#pragma once
#include <stdint.h>

#include <memory>

#include "AbstractContainerMenu.h"
#include "java/Random.h"

class Container;
class Inventory;
class Level;

class EnchantmentMenu : public AbstractContainerMenu {
    // 4J Stu Made these public for UI menus, perhaps should make friend class?
public:
    static const int INGREDIENT_SLOT = 0;
    static const int INV_SLOT_START = EnchantmentMenu::INGREDIENT_SLOT + 1;
    static const int INV_SLOT_END = EnchantmentMenu::INV_SLOT_START + 9 * 3;
    static const int USE_ROW_SLOT_START = EnchantmentMenu::INV_SLOT_END;
    static const int USE_ROW_SLOT_END = EnchantmentMenu::USE_ROW_SLOT_START + 9;

public:
    std::shared_ptr<Container> enchantSlots;

private:
    Level* level;
    int x, y, z;
    Random random;

    bool m_costsChanged;  // 4J Added

public:
    int64_t nameSeed;

public:
    int costs[3];

    EnchantmentMenu(std::shared_ptr<Inventory> inventory, Level* level, int xt,
                    int yt, int zt);

    virtual void addSlotListener(ContainerListener* listener);
    virtual void broadcastChanges();
    virtual void setData(int id, int value);
    virtual void
    slotsChanged();  // 4J used to take a std::shared_ptr<Container> container
                     // but wasn't using it, so removed to simplify things
    virtual bool clickMenuButton(std::shared_ptr<Player> player, int i);
    void removed(std::shared_ptr<Player> player);
    virtual bool stillValid(std::shared_ptr<Player> player);
    virtual std::shared_ptr<ItemInstance> quickMoveStack(
        std::shared_ptr<Player> player, int slotIndex);
};