#pragma once
#include <cstdint>
#include <memory>
#include <string>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class TexturePacket : public Packet,
                      public std::enable_shared_from_this<TexturePacket> {
public:
    std::string textureName;
    std::uint8_t* pbData;
    std::uint32_t dataBytes;

    TexturePacket();
    ~TexturePacket();
    TexturePacket(const std::string& textureName, std::uint8_t* pbData,
                  std::uint32_t dataBytes);

    virtual void handle(PacketListener* listener);
    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<TexturePacket>();
    }
    virtual int getId() { return 154; }
};
