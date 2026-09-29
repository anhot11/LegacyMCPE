#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class LevelSoundPacket : public Packet,
                         public std::enable_shared_from_this<LevelSoundPacket> {
public:
    static const float PITCH_ACCURACY;
    static const float LOCATION_ACCURACY;

private:
    int sound;
    int x;
    int y;
    int z;
    float volume;
    // int pitch;
    float pitch;

public:
    LevelSoundPacket();
    LevelSoundPacket(int iSound, double x, double y, double z, float volume,
                     float pitch);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual int getSound();
    double getX();
    double getY();
    double getZ();
    float getVolume();
    float getPitch();
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<LevelSoundPacket>();
    }
    virtual int getId() { return 62; }
};
