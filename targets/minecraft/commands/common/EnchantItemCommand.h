#pragma once

#include <stdint.h>

#include <memory>

#include "minecraft/commands/Command.h"
#include "minecraft/commands/CommandsEnum.h"

class GameCommandPacket;
class Player;

class EnchantItemCommand : public Command {
public:
    virtual EGameCommand getId();
    int getPermissionLevel();
    virtual void execute(std::shared_ptr<CommandSender> source,
                         std::vector<uint8_t>& commandData);

    static std::shared_ptr<GameCommandPacket> preparePacket(
        std::shared_ptr<Player> player, int enchantmentId,
        int enchantmentLevel = 1);
};