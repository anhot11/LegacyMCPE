#pragma once
// 4J Stu Added
// In InventoryMenu.java they create an anoymous class while creating some slot.
// I have moved the content of that anonymous class to here

#include <memory>

#include "Slot.h"

class Container;

class ArmorSlot : public Slot {
private:
    const int slotNum;

public:
    ArmorSlot(int slotNum, std::shared_ptr<Container> container, int id, int x,
              int y);
    virtual ~ArmorSlot() {}

    virtual int getMaxStackSize();
    virtual bool mayPlace(std::shared_ptr<ItemInstance> item);
    Icon* getNoItemIcon();
    // virtual bool mayCombine(std::shared_ptr<ItemInstance> item); // 4J Added
    // virtual std::shared_ptr<ItemInstance>
    // combine(std::shared_ptr<ItemInstance> item); // 4J Added
};