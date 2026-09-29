#pragma once

#include <memory>

#include "ComplexItem.h"

class MapItemSavedData;
class Entity;
class ItemInstance;
class Level;

class MapItem : public ComplexItem {
public:
    static const int IMAGE_WIDTH = 128;
    static const int IMAGE_HEIGHT = 128;

public:  // 4J Stu - Was protected in Java, but then we can't access it where we
         // need it
    MapItem(int id);

    static std::shared_ptr<MapItemSavedData> getSavedData(short idNum,
                                                          Level* level);
    std::shared_ptr<MapItemSavedData> getSavedData(
        std::shared_ptr<ItemInstance> itemInstance, Level* level);
    void update(Level* level, std::shared_ptr<Entity> player,
                std::shared_ptr<MapItemSavedData> data);
    virtual void inventoryTick(std::shared_ptr<ItemInstance> itemInstance,
                               Level* level, std::shared_ptr<Entity> owner,
                               int slot, bool selected);
    std::shared_ptr<Packet> getUpdatePacket(
        std::shared_ptr<ItemInstance> itemInstance, Level* level,
        std::shared_ptr<Player> player);
    virtual void onCraftedBy(std::shared_ptr<ItemInstance> itemInstance,
                             Level* level, std::shared_ptr<Player> player);
};
