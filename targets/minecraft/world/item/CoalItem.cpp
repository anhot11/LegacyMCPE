#include "CoalItem.h"

#include <memory>
#include <string>

#include "minecraft/world/IconRegister.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "strings.h"

CoalItem::CoalItem(int id) : Item(id) {
    setStackedByData(true);
    setMaxDamage(0);
}

unsigned int CoalItem::getDescriptionId(
    std::shared_ptr<ItemInstance> instance) {
    if (instance->getAuxValue() == CHAR_COAL) {
        return IDS_ITEM_CHARCOAL;
    }
    return IDS_ITEM_COAL;
}

Icon* CoalItem::getIcon(int auxValue) {
    if (auxValue == CHAR_COAL) {
        return charcoalIcon;
    }
    return Item::getIcon(auxValue);
}

void CoalItem::registerIcons(IconRegister* iconRegister) {
    Item::registerIcons(iconRegister);

    charcoalIcon = iconRegister->registerIcon("charcoal");
}