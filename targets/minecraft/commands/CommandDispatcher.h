#pragma once

#include <stdint.h>

#include <format>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "minecraft/commands/CommandsEnum.h"

class Command;
class CommandSender;

class CommandDispatcher {
private:
    std::unordered_map<EGameCommand, Command*> commandsById;
    std::unordered_set<Command*> commands;

public:
    int performCommand(std::shared_ptr<CommandSender> sender,
                       EGameCommand command, std::vector<uint8_t>& commandData);
    Command* addCommand(Command* command);
};