#include "ComplexItem.h"

#include "minecraft/world/item/Item.h"

class ItemInstance;

ComplexItem::ComplexItem(int id) : Item(id) {}

bool ComplexItem::isComplex() { return true; }

std::shared_ptr<Packet> ComplexItem::getUpdatePacket(
    std::shared_ptr<ItemInstance> itemInstance, Level* level,
    std::shared_ptr<Player> player) {
    return nullptr;
}