#include "ToggleDownfallCommand.h"

#include <string>
#include <vector>

#include "minecraft/commands/CommandsEnum.h"
#include "minecraft/network/packet/ChatPacket.h"
#include "minecraft/network/packet/GameCommandPacket.h"
#include "minecraft/server/MinecraftServer.h"
#include "minecraft/server/level/ServerLevel.h"
#include "minecraft/world/level/storage/LevelData.h"

EGameCommand ToggleDownfallCommand::getId() {
    return eGameCommand_ToggleDownfall;
}

int ToggleDownfallCommand::getPermissionLevel() { return LEVEL_GAMEMASTERS; }

void ToggleDownfallCommand::execute(std::shared_ptr<CommandSender> source,
                                    std::vector<uint8_t>& commandData) {
    doToggleDownfall();
    logAdminAction(source, ChatPacket::e_ChatCustom,
                   "commands.downfall.success");
}

void ToggleDownfallCommand::doToggleDownfall() {
    MinecraftServer::getInstance()->levels[0]->toggleDownfall();
    MinecraftServer::getInstance()->levels[0]->getLevelData()->setThundering(
        true);
}

std::shared_ptr<GameCommandPacket> ToggleDownfallCommand::preparePacket() {
    return std::shared_ptr<GameCommandPacket>(new GameCommandPacket(
        eGameCommand_ToggleDownfall, std::vector<uint8_t>()));
}