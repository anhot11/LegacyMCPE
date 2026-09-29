#pragma once

#include <stdint.h>

#include "minecraft/commands/Command.h"
#include "minecraft/commands/CommandsEnum.h"

class CommandSender;

class ExperienceCommand : public Command {
public:
    virtual EGameCommand getId();
    virtual int getPermissionLevel();
    virtual void execute(std::shared_ptr<CommandSender> source,
                         std::vector<uint8_t>& commandData);
};