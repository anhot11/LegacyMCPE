#pragma once

#include <format>
#include <memory>
#include <vector>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"
#include "minecraft/world/entity/SyncedEntityData.h"

class SetEntityDataPacket
    : public Packet,
      public std::enable_shared_from_this<SetEntityDataPacket> {
public:
    int id;

private:
    std::vector<std::shared_ptr<SynchedEntityData::DataItem> >* packedItems;

public:
    SetEntityDataPacket();
    ~SetEntityDataPacket();
    SetEntityDataPacket(int id, std::shared_ptr<SynchedEntityData>,
                        bool notJustDirty);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

    std::vector<std::shared_ptr<SynchedEntityData::DataItem> >*
    getUnpackedData();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<SetEntityDataPacket>();
    }
    virtual int getId() { return 40; }
};