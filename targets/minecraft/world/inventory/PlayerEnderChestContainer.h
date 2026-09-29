#pragma once

#include <memory>

#include "minecraft/world/SimpleContainer.h"
#include "nbt/CompoundTag.h"

class ItemInstance;
class EnderChestTileEntity;
class CompoundTag;
template <class T>
class ListTag;

class PlayerEnderChestContainer : public SimpleContainer {
private:
    std::shared_ptr<EnderChestTileEntity> activeChest;

public:
    PlayerEnderChestContainer();

    virtual int getContainerType();

    void setActiveChest(std::shared_ptr<EnderChestTileEntity> activeChest);
    void setItemsByTag(ListTag<CompoundTag>* enderItemsList);
    ListTag<CompoundTag>* createTag();
    bool stillValid(std::shared_ptr<Player> player);
    void startOpen();
    void stopOpen();
    bool canPlaceItem(int slot, std::shared_ptr<ItemInstance> item);
};