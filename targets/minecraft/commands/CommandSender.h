#pragma once

#include "CommandsEnum.h"
#include "minecraft/network/packet/ChatPacket.h"

class CommandSender {
public:
    // virtual int getUID() = 0;
    virtual void sendMessage(
        const std::string& message,
        ChatPacket::EChatPacketMessage type = ChatPacket::e_ChatCustom,
        int customData = -1, const std::string& additionalMessage = "") = 0;
    virtual bool hasPermission(EGameCommand command) = 0;
};