#pragma once

#include <cstdint>
#include <memory>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class ServerPlayer;

class PlayerInfoPacket : public Packet,
                         public std::enable_shared_from_this<PlayerInfoPacket> {
public:
    // 4J Stu - I have re-purposed this packet for our uses
    // std::string name;
    // bool add;
    // int latency;
    short m_networkSmallId;
    short m_playerColourIndex;
    unsigned int m_playerPrivileges;
    int m_entityId;

    PlayerInfoPacket();
    // PlayerInfoPacket(const std::string &name, bool add, int latency);
    PlayerInfoPacket(std::uint8_t networkSmallId, short playerColourIndex,
                     unsigned int playerPrivileges = 0);
    PlayerInfoPacket(std::shared_ptr<ServerPlayer> player);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<PlayerInfoPacket>();
    }
    virtual int getId() { return 201; }
};