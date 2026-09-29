#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class Entity;

class PlayerCommandPacket
    : public Packet,
      public std::enable_shared_from_this<PlayerCommandPacket> {
public:
    static const int START_SNEAKING;
    static const int STOP_SNEAKING;
    static const int STOP_SLEEPING;
    static const int START_SPRINTING;
    static const int STOP_SPRINTING;
    static const int START_IDLEANIM;
    static const int STOP_IDLEANIM;
    static const int RIDING_JUMP;
    static const int OPEN_INVENTORY;

    // 4J Added
    // 4J-PB - Making this host only setting
    /*
    static const int SHOW_ON_MAPS;
    static const int HIDE_ON_MAPS;
    */

    int id;
    int action;
    int data;

    PlayerCommandPacket();
    PlayerCommandPacket(std::shared_ptr<Entity> e, int action);
    PlayerCommandPacket(std::shared_ptr<Entity> e, int action, int data);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<PlayerCommandPacket>();
    }
    virtual int getId() { return 19; }
};