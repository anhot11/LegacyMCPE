#pragma once

#include <format>
#include <memory>
#include <string>
#include <vector>

#include "Packet.h"
#include "minecraft/network/packet/Packet.h"

class PlayerTeam;

class SetPlayerTeamPacket
    : public Packet,
      public std::enable_shared_from_this<SetPlayerTeamPacket> {
public:
    static const int METHOD_ADD = 0;
    static const int METHOD_REMOVE = 1;
    static const int METHOD_CHANGE = 2;
    static const int METHOD_JOIN = 3;
    static const int METHOD_LEAVE = 4;

    std::string name;
    std::string displayName;
    std::string prefix;
    std::string suffix;
    std::vector<std::string> players;
    int method;
    int options;

    SetPlayerTeamPacket();
    SetPlayerTeamPacket(PlayerTeam* team, int method);
    SetPlayerTeamPacket(PlayerTeam* team, std::vector<std::string>* players,
                        int method);
    void read(DataInputStream* dis);
    void write(DataOutputStream* dos);
    void handle(PacketListener* listener);
    int getEstimatedSize();

public:
    static std::shared_ptr<Packet> create() {
        return std::make_shared<SetPlayerTeamPacket>();
    }
    virtual int getId() { return 209; }
};