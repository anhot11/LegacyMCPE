#pragma once

#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class ClientCommandPacket
    : public Packet,
      public std::enable_shared_from_this<ClientCommandPacket> {
public:
    static const int LOGIN_COMPLETE = 0;
    static const int PERFORM_RESPAWN = 1;

    int action;

    ClientCommandPacket();
    ClientCommandPacket(int action);

    void read(DataInputStream* dis);
    void write(DataOutputStream* dos);
    void handle(PacketListener* listener);
    int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<ClientCommandPacket>();
    }
    virtual int getId() { return 205; }
};