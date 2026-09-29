#pragma once

#include <memory>

#include "Item.h"

class Player;
class Packet;
class Level;
class ItemInstance;

class ComplexItem : public Item {
protected:
    ComplexItem(int id);

public:
    virtual bool isComplex();
    virtual std::shared_ptr<Packet> getUpdatePacket(
        std::shared_ptr<ItemInstance> itemInstance, Level* level,
        std::shared_ptr<Player> player);
};
