#pragma once

#include <stdint.h>

#include <format>
#include <memory>
#include <string>
#include <vector>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class CustomPayloadPacket
    : public Packet,
      public std::enable_shared_from_this<CustomPayloadPacket> {
public:
    // Mojang-defined custom packets
    static const std::string CUSTOM_BOOK_PACKET;
    static const std::string CUSTOM_BOOK_SIGN_PACKET;
    static const std::string TEXTURE_PACK_PACKET;
    static const std::string TRADER_LIST_PACKET;
    static const std::string TRADER_SELECTION_PACKET;
    static const std::string SET_ADVENTURE_COMMAND_PACKET;
    static const std::string SET_BEACON_PACKET;
    static const std::string SET_ITEM_NAME_PACKET;

    std::string identifier;
    int length;
    std::vector<uint8_t> data;

    CustomPayloadPacket();
    CustomPayloadPacket(const std::string& identifier,
                        std::vector<uint8_t> data);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<CustomPayloadPacket>();
    }
    virtual int getId() { return 250; }
};