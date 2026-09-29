#pragma once

#include "Container.h"

class WorldlyContainer : public Container {
public:
    virtual std::vector<int> getSlotsForFace(int face) = 0;
    virtual bool canPlaceItemThroughFace(int slot,
                                         std::shared_ptr<ItemInstance> item,
                                         int face) = 0;
    virtual bool canTakeItemThroughFace(int slot,
                                        std::shared_ptr<ItemInstance> item,
                                        int face) = 0;
};