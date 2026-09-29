#include "RepairContainer.h"

#include <string>

#include "AnvilMenu.h"
#include "minecraft/world/SimpleContainer.h"

RepairContainer::RepairContainer(AnvilMenu* menu, int name, bool customName,
                                 int size)
    : SimpleContainer(name, "", customName, size) {
    m_menu = menu;
}

void RepairContainer::setChanged() {
    SimpleContainer::setChanged();
    m_menu->slotsChanged(shared_from_this());
}

bool RepairContainer::canPlaceItem(int slot,
                                   std::shared_ptr<ItemInstance> item) {
    return true;
}