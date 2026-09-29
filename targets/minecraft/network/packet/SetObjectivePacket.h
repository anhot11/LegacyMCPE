#pragma once

#include <memory>
#include <string>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class Objective;

class SetObjectivePacket
    : public Packet,
      public std::enable_shared_from_this<SetObjectivePacket> {
public:
    static const int METHOD_ADD = 0;
    static const int METHOD_REMOVE = 1;
    static const int METHOD_CHANGE = 2;

    std::string objectiveName;
    std::string displayName;
    int method;

    SetObjectivePacket();
    SetObjectivePacket(Objective* objective, int method);
    void read(DataInputStream* dis);
    void write(DataOutputStream* dos);
    void handle(PacketListener* listener);
    int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<SetObjectivePacket>();
    }
    virtual int getId() { return 206; }
};