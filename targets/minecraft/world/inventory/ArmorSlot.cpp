#include "ArmorSlot.h"

#include "minecraft/world/inventory/Slot.h"
#include "minecraft/world/item/ArmorItem.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/tile/Tile.h"

ArmorSlot::ArmorSlot(int slotNum, std::shared_ptr<Container> container, int id,
                     int x, int y)
    : Slot(container, id, x, y), slotNum(slotNum) {}

int ArmorSlot::getMaxStackSize() { return 1; }

bool ArmorSlot::mayPlace(std::shared_ptr<ItemInstance> item) {
    if (item == nullptr) {
        return false;
    }
    if (dynamic_cast<ArmorItem*>(item->getItem()) != nullptr) {
        return dynamic_cast<ArmorItem*>(item->getItem())->slot == slotNum;
    }
    if (item->getItem()->id == Tile::pumpkin_Id ||
        item->getItem()->id == Item::skull_Id) {
        return slotNum == 0;
    }
    return false;
}

Icon* ArmorSlot::getNoItemIcon() { return ArmorItem::getEmptyIcon(slotNum); }

//
// bool ArmorSlot::mayCombine(shared_ptr<ItemInstance> item)
//{
//	shared_ptr<ItemInstance> thisItemI = getItem();
//	if(thisItemI == nullptr || item == nullptr) return false;
//
//	ArmorItem *thisItem = (ArmorItem *)thisItemI->getItem();
//	bool thisIsDyableArmor = thisItem->getMaterial() ==
// ArmorItem::ArmorMaterial::CLOTH; 	bool itemIsDye = item->id ==
// Item::dye_powder_Id; 	return thisIsDyableArmor && itemIsDye;
//}
//
// shared_ptr<ItemInstance> ArmorSlot::combine(shared_ptr<ItemInstance> item)
//{
//	shared_ptr<CraftingContainer> craftSlots =
// shared_ptr<CraftingContainer>( new CraftingContainer(nullptr, 2, 2) );
//	craftSlots->setItem(0, item);
//	craftSlots->setItem(1, getItem()); // Armour item needs to go second
//	shared_ptr<ItemInstance> result =
// ArmorDyeRecipe::assembleDyedArmor(craftSlots); 	craftSlots->setItem(0,
// nullptr); 	craftSlots->setItem(1, nullptr); 	return result;
//}