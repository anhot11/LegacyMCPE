#pragma once

#include <stdint.h>

#include <format>
#include <memory>
#include <vector>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"
#include "minecraft/world/entity/SyncedEntityData.h"

class LivingEntity;

class AddMobPacket : public Packet,
                     public std::enable_shared_from_this<AddMobPacket> {
public:
    int id;
    int type;
    int x, y, z;
    int xd, yd, zd;
    uint8_t yRot, xRot, yHeadRot;

private:
    std::shared_ptr<SynchedEntityData> entityData;
    std::vector<std::shared_ptr<SynchedEntityData::DataItem> >* unpack;

public:
    AddMobPacket();
    ~AddMobPacket();
    AddMobPacket(std::shared_ptr<LivingEntity> mob, int yRotp, int xRotp,
                 int xp, int yp, int zp, int yHeadRotp);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

    std::vector<std::shared_ptr<SynchedEntityData::DataItem> >*
    getUnpackedData();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<AddMobPacket>();
    }
    virtual int getId() { return 24; }
};
