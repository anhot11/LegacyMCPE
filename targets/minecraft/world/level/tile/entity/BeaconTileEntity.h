#pragma once

#include <stdint.h>

#include <memory>
#include <string>

#include "TileEntity.h"
#include "java/Class.h"
#include "minecraft/SharedConstants.h"
#include "minecraft/world/Container.h"

class MobEffect;
class ItemInstance;

class BeaconTileEntity : public TileEntity, public Container {
public:
    eINSTANCEOF GetType() { return eTYPE_BEACONTILEENTITY; }
    static TileEntity* create() { return new BeaconTileEntity(); }
    // 4J Added
    virtual std::shared_ptr<TileEntity> clone();

private:
    static const int SCALE_TIME = SharedConstants::TICKS_PER_SECOND * 2;

public:
    static const int BEACON_EFFECTS_TIERS = 4;
    static const int BEACON_EFFECTS_EFFECTS = 3;
    static MobEffect* BEACON_EFFECTS[BEACON_EFFECTS_TIERS]
                                    [BEACON_EFFECTS_EFFECTS];

    static void staticCtor();

private:
    int64_t clientSideRenderTick;
    float clientSideRenderScale;

    bool isActive;
    int levels;

    int primaryPower;
    int secondaryPower;

    std::shared_ptr<ItemInstance> paymentItem;
    std::string name;

public:
    BeaconTileEntity();

    void tick();

private:
    void applyEffects();
    void updateShape();

public:
    float getAndUpdateClientSideScale();
    int getPrimaryPower();
    int getSecondaryPower();
    int getLevels();
    // client-side method used by GUI
    void setLevels(int levels);
    void setPrimaryPower(int primaryPower);
    void setSecondaryPower(int secondaryPower);
    std::shared_ptr<Packet> getUpdatePacket();
    double getViewDistance();
    void load(CompoundTag* tag);
    void save(CompoundTag* tag);
    unsigned int getContainerSize();
    std::shared_ptr<ItemInstance> getItem(unsigned int slot);
    std::shared_ptr<ItemInstance> removeItem(unsigned int slot, int count);
    std::shared_ptr<ItemInstance> removeItemNoUpdate(int slot);
    void setItem(unsigned int slot, std::shared_ptr<ItemInstance> item);
    std::string getName();
    std::string getCustomName();
    bool hasCustomName();
    void setCustomName(const std::string& name);
    int getMaxStackSize();
    bool stillValid(std::shared_ptr<Player> player);
    void startOpen();
    void stopOpen();
    bool canPlaceItem(int slot, std::shared_ptr<ItemInstance> item);

    // 4J Stu - For container
    virtual void setChanged() { TileEntity::setChanged(); }
};