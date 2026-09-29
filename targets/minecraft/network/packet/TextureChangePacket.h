#pragma once

#include <memory>
#include <string>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class Entity;

class TextureChangePacket
    : public Packet,
      public std::enable_shared_from_this<TextureChangePacket> {
public:
    enum ETextureChangeType {
        e_TextureChange_Skin = 0,
        e_TextureChange_Cape,
    };

    int id;
    ETextureChangeType action;
    std::string path;

    TextureChangePacket();
    TextureChangePacket(std::shared_ptr<Entity> e, ETextureChangeType action,
                        const std::string& path);

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<TextureChangePacket>();
    }
    virtual int getId() { return 157; }
};