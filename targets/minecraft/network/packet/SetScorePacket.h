#pragma once

#include <memory>
#include <string>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class Score;

class SetScorePacket : public Packet,
                       public std::enable_shared_from_this<SetScorePacket> {
public:
    static const int METHOD_CHANGE = 0;
    static const int METHOD_REMOVE = 1;

    std::string owner;
    std::string objectiveName;
    int score;
    int method;

    SetScorePacket();
    SetScorePacket(Score* score, int method);
    SetScorePacket(const std::string& owner);

    void read(DataInputStream* dis);
    void write(DataOutputStream* dos);
    void handle(PacketListener* listener);
    int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<SetScorePacket>();
    }
    virtual int getId() { return 207; }
};