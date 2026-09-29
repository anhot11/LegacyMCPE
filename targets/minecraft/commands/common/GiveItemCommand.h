#pragma once

#include <stdint.h>

#include <memory>
#include <string>

#include "minecraft/commands/Command.h"
#include "minecraft/commands/CommandsEnum.h"

class GameCommandPacket;
class Player;

class GiveItemCommand : public Command {
public:
    virtual EGameCommand getId();
    virtual int getPermissionLevel();
    virtual void execute(std::shared_ptr<CommandSender> source,
                         std::vector<uint8_t>& commandData);

public:
    static std::shared_ptr<GameCommandPacket> preparePacket(
        std::shared_ptr<Player> player, int item, int amount = 1, int aux = 0,
        const std::string& tag = "");
};