#include "PlayerEnderChestContainer.h"

#include <stdint.h>

#include <string>

#include "minecraft/network/packet/ContainerOpenPacket.h"
#include "minecraft/world/SimpleContainer.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/tile/entity/EnderChestTileEntity.h"
#include "nbt/CompoundTag.h"
#include "nbt/ListTag.h"
#include "strings.h"

PlayerEnderChestContainer::PlayerEnderChestContainer()
    : SimpleContainer(IDS_TILE_ENDERCHEST, "", false, 9 * 3) {
    activeChest = nullptr;
}

int PlayerEnderChestContainer::getContainerType() {
    return ContainerOpenPacket::ENDER_CHEST;
}

void PlayerEnderChestContainer::setActiveChest(
    std::shared_ptr<EnderChestTileEntity> activeChest) {
    this->activeChest = activeChest;
}

void PlayerEnderChestContainer::setItemsByTag(
    ListTag<CompoundTag>* enderItemsList) {
    for (int i = 0; i < getContainerSize(); i++) {
        setItem(i, nullptr);
    }
    for (int i = 0; i < enderItemsList->size(); i++) {
        CompoundTag* tag = enderItemsList->get(i);
        int slot = tag->getByte("Slot") & 0xff;
        if (slot >= 0 && slot < getContainerSize())
            setItem(slot, ItemInstance::fromTag(tag));
    }
}

ListTag<CompoundTag>* PlayerEnderChestContainer::createTag() {
    ListTag<CompoundTag>* items = new ListTag<CompoundTag>("EnderItems");
    for (int i = 0; i < getContainerSize(); i++) {
        std::shared_ptr<ItemInstance> item = getItem(i);
        if (item != nullptr) {
            CompoundTag* tag = new CompoundTag();
            tag->putByte("Slot", (uint8_t)i);
            item->save(tag);
            items->add(tag);
        }
    }
    return items;
}

bool PlayerEnderChestContainer::stillValid(std::shared_ptr<Player> player) {
    if (activeChest != nullptr && !activeChest->stillValid(player)) {
        return false;
    }
    return SimpleContainer::stillValid(player);
}

void PlayerEnderChestContainer::startOpen() {
    if (activeChest != nullptr) {
        activeChest->startOpen();
    }
    SimpleContainer::startOpen();
}

void PlayerEnderChestContainer::stopOpen() {
    if (activeChest) {
        activeChest->stopOpen();
    }
    SimpleContainer::stopOpen();
    activeChest = nullptr;
}

bool PlayerEnderChestContainer::canPlaceItem(
    int slot, std::shared_ptr<ItemInstance> item) {
    return true;
}