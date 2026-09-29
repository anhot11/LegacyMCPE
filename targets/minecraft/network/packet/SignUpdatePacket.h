#pragma once

#include <memory>
#include <string>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class SignUpdatePacket : public Packet,
                         public std::enable_shared_from_this<SignUpdatePacket> {
public:
    int x, y, z;
    bool m_bVerified;
    bool m_bCensored;
    std::string lines[4];

    SignUpdatePacket();
    SignUpdatePacket(int x, int y, int z, bool bVerified, bool bCensored,
                     std::string lines[]);
    bool GetVerified() { return m_bVerified; }
    bool GetCensored() { return m_bCensored; }
    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<SignUpdatePacket>();
    }
    virtual int getId() { return 130; }
};