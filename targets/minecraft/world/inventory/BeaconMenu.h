#pragma once

#include <memory>

#include "AbstractContainerMenu.h"
#include "Slot.h"

class BeaconTileEntity;
class Container;

class BeaconMenu : public AbstractContainerMenu {
private:
    class PaymentSlot : public Slot {
    public:
        PaymentSlot(std::shared_ptr<Container> container, int slot, int x,
                    int y);

        bool mayPlace(std::shared_ptr<ItemInstance> item);
        int getMaxStackSize();
    };

public:
    static const int PAYMENT_SLOT = 0;
    static const int INV_SLOT_START = PAYMENT_SLOT + 1;
    static const int INV_SLOT_END = INV_SLOT_START + 9 * 3;
    static const int USE_ROW_SLOT_START = INV_SLOT_END;
    static const int USE_ROW_SLOT_END = USE_ROW_SLOT_START + 9;

private:
    std::shared_ptr<BeaconTileEntity> beacon;
    PaymentSlot* paymentSlot;

    // copied values because container/client system is retarded
    int levels;
    int primaryPower;
    int secondaryPower;

public:
    BeaconMenu(std::shared_ptr<Container> inventory,
               std::shared_ptr<BeaconTileEntity> beacon);

    void addSlotListener(ContainerListener* listener);
    void setData(int id, int value);
    std::shared_ptr<BeaconTileEntity> getBeacon();
    bool stillValid(std::shared_ptr<Player> player);
    std::shared_ptr<ItemInstance> quickMoveStack(std::shared_ptr<Player> player,
                                                 int slotIndex);
};