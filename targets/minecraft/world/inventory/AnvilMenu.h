#pragma once

#include <memory>
#include <string>

#include "AbstractContainerMenu.h"

class Container;
class Inventory;
class Level;
class Player;

class AnvilMenu : public AbstractContainerMenu {
    friend class RepairResultSlot;

private:
    static const bool DEBUG_COST = false;

public:
    static const int INPUT_SLOT = 0;
    static const int ADDITIONAL_SLOT = 1;
    static const int RESULT_SLOT = 2;

    static const int INV_SLOT_START = RESULT_SLOT + 1;
    static const int INV_SLOT_END = INV_SLOT_START + 9 * 3;
    static const int USE_ROW_SLOT_START = INV_SLOT_END;
    static const int USE_ROW_SLOT_END = USE_ROW_SLOT_START + 9;

public:
    static const int DATA_TOTAL_COST = 0;

private:
    std::shared_ptr<Container> resultSlots;

    // 4J Stu - anonymous class here now RepairContainer
    std::shared_ptr<Container> repairSlots;

    Level* level;
    int x, y, z;

public:
    int cost;

private:
    int repairItemCountCost;
    std::string itemName;
    std::shared_ptr<Player> player;

public:
    using AbstractContainerMenu::slotsChanged;

    AnvilMenu(std::shared_ptr<Inventory> inventory, Level* level, int xt,
              int yt, int zt, std::shared_ptr<Player> player);

    void slotsChanged(std::shared_ptr<Container> container);
    void createResult();
    void sendData(int id, int value);
    void addSlotListener(ContainerListener* listener);
    void setData(int id, int value);
    void removed(std::shared_ptr<Player> player);
    bool stillValid(std::shared_ptr<Player> player);
    std::shared_ptr<ItemInstance> quickMoveStack(std::shared_ptr<Player> player,
                                                 int slotIndex);
    void setItemName(const std::string& name);
};
