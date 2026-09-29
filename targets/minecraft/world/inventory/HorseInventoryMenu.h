#pragma once

#include <memory>

#include "AbstractContainerMenu.h"
#include "Slot.h"
#include "minecraft/world/Container.h"

class HorseInventoryMenu;
class EntityHorse;

class HorseSaddleSlot : public Slot {
public:
    HorseSaddleSlot(std::shared_ptr<Container> horseInventory);

    bool mayPlace(std::shared_ptr<ItemInstance> item);
};

class HorseArmorSlot : public Slot {
private:
    HorseInventoryMenu* m_parent;

public:
    HorseArmorSlot(HorseInventoryMenu* parent,
                   std::shared_ptr<Container> horseInventory);

    bool mayPlace(std::shared_ptr<ItemInstance> item);
    bool isActive();
};

class HorseInventoryMenu : public AbstractContainerMenu {
    friend class HorseArmorSlot;

private:
    std::shared_ptr<Container> horseContainer;
    std::shared_ptr<EntityHorse> horse;

public:
    HorseInventoryMenu(std::shared_ptr<Container> playerInventory,
                       std::shared_ptr<Container> horseInventory,
                       std::shared_ptr<EntityHorse> horse);

    bool stillValid(std::shared_ptr<Player> player);
    std::shared_ptr<ItemInstance> quickMoveStack(std::shared_ptr<Player> player,
                                                 int slotIndex);
    void removed(std::shared_ptr<Player> player);
    std::shared_ptr<Container> getContainer();
};