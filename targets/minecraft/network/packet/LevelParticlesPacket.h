#pragma once

#include <memory>
#include <string>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class LevelParticlesPacket
    : public Packet,
      public std::enable_shared_from_this<LevelParticlesPacket> {
private:
    std::string name;
    float x;
    float y;
    float z;
    float xDist;
    float yDist;
    float zDist;
    float maxSpeed;
    int count;

public:
    LevelParticlesPacket();
    LevelParticlesPacket(const std::string& name, float x, float y, float z,
                         float xDist, float yDist, float zDist, float maxSpeed,
                         int count);

    void read(DataInputStream* dis);
    void write(DataOutputStream* dos);
    std::string getName();
    double getX();
    double getY();
    double getZ();
    float getXDist();
    float getYDist();
    float getZDist();
    float getMaxSpeed();
    int getCount();
    void handle(PacketListener* listener);
    int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<LevelParticlesPacket>();
    }
    virtual int getId() { return 63; }
};