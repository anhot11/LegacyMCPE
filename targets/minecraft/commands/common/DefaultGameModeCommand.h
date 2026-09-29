#pragma once

#include <stdint.h>

#include "GameModeCommand.h"
#include "minecraft/commands/CommandsEnum.h"

class GameType;

class DefaultGameModeCommand : public GameModeCommand {
public:
    virtual EGameCommand getId();
    virtual void execute(std::shared_ptr<CommandSender> source,
                         std::vector<uint8_t>& commandData);

protected:
    void doSetGameType(GameType* newGameType);
};