#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class PlayerInputPacket
    : public Packet,
      public std::enable_shared_from_this<PlayerInputPacket> {
private:
    float xxa;
    float yya;
    bool isJumpingVar;
    bool isSneakingVar;

public:
    PlayerInputPacket();
    PlayerInputPacket(float xxa, float yya, bool isJumpingVar,
                      bool isSneakingVar);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

    float getXxa();
    float getYya();
    bool isJumping();
    bool isSneaking();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<PlayerInputPacket>();
    }
    virtual int getId() { return 27; }
};