#pragma once

#include <stdint.h>

#include <format>
#include <memory>
#include <vector>

#include "Packet.h"
#include "minecraft/commands/CommandsEnum.h"
#include "minecraft/network/packet/Packet.h"

class GameCommandPacket
    : public Packet,
      public std::enable_shared_from_this<GameCommandPacket> {
public:
    EGameCommand command;
    int length;
    std::vector<uint8_t> data;

    GameCommandPacket();
    GameCommandPacket(EGameCommand command, std::vector<uint8_t> data);
    ~GameCommandPacket();

    virtual void read(DataInputStream* dis);
    virtual void write(DataOutputStream* dos);
    virtual void handle(PacketListener* listener);
    virtual int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<GameCommandPacket>();
    }
    virtual int getId() { return 167; }
};