#include "DefaultGameModeCommand.h"

#include <memory>

#include "minecraft/commands/CommandsEnum.h"

EGameCommand DefaultGameModeCommand::getId() {
    return eGameCommand_DefaultGameMode;
}

void DefaultGameModeCommand::execute(std::shared_ptr<CommandSender> source,
                                     std::vector<uint8_t>& commandData) {
    // if (args.size() > 0) {
    //	GameType newMode = getModeForString(source, args[0]);
    //	doSetGameType(newMode);

    //	logAdminAction(source, "commands.defaultgamemode.success",
    // ChatMessageComponent.forTranslation("gameMode." + newMode.getName()));

    //	return;
    //}

    // throw new UsageException("commands.defaultgamemode.usage");
}

void DefaultGameModeCommand::doSetGameType(GameType* newGameType) {
    // MinecraftServer minecraftServer = MinecraftServer.getInstance();
    // minecraftServer.setDefaultGameMode(newGameType);

    // if (minecraftServer.getForceGameType()) {
    //	for (ServerPlayer player :
    // MinecraftServer.getInstance().getPlayers().players) {
    //		player.setGameMode(newGameType);
    //		player.fallDistance = 0; // reset falldistance so flying people
    // do not die :P
    //	}
    // }
}