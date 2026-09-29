#include "EnchantmentContainer.h"

#include <memory>
#include <string>

#include "minecraft/world/SimpleContainer.h"
#include "minecraft/world/inventory/EnchantmentMenu.h"
#include "strings.h"

EnchantmentContainer::EnchantmentContainer(EnchantmentMenu* menu)
    : SimpleContainer(IDS_ENCHANT, "", false, 1), m_menu(menu) {}

int EnchantmentContainer::getMaxStackSize() { return 1; }

void EnchantmentContainer::setChanged() {
    SimpleContainer::setChanged();
    m_menu->slotsChanged();  // Remove this param as it's not needed
}

bool EnchantmentContainer::canPlaceItem(int slot,
                                        std::shared_ptr<ItemInstance> item) {
    return true;
}