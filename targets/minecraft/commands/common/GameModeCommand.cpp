#include "GameModeCommand.h"

#include "minecraft/commands/CommandsEnum.h"

class CommandSender;

EGameCommand GameModeCommand::getId() { return eGameCommand_GameMode; }

int GameModeCommand::getPermissionLevel() { return LEVEL_GAMEMASTERS; }

void GameModeCommand::execute(std::shared_ptr<CommandSender> source,
                              std::vector<uint8_t>& commandData) {
    // if (args.size() > 0) {
    //	GameType newMode = getModeForString(source, args[0]);
    //	Player player = args.size() >= 2 ? convertToPlayer(source, args[1]) :
    // convertSourceToPlayer(source);

    //	player.setGameMode(newMode);
    //	player.fallDistance = 0; // reset falldistance so flying people do not
    // die :P

    //	ChatMessageComponent mode =
    // ChatMessageComponent.forTranslation("gameMode." + newMode.getName());

    //	if (player != source) {
    //		logAdminAction(source,
    // AdminLogCommand.LOGTYPE_DONT_SHOW_TO_SELF,
    //"commands.gamemode.success.other", player.getAName(), mode); 	} else {
    //		logAdminAction(source,
    // AdminLogCommand.LOGTYPE_DONT_SHOW_TO_SELF,
    //"commands.gamemode.success.self", mode);
    //	}

    //	return;
    //}

    // throw new UsageException("commands.gamemode.usage");
}

GameType* GameModeCommand::getModeForString(
    std::shared_ptr<CommandSender> source, const std::string& name) {
    return nullptr;
    // if (name.equalsIgnoreCase(GameType.SURVIVAL.getName()) ||
    // name.equalsIgnoreCase("s")) { 	return GameType.SURVIVAL; } else if
    // (name.equalsIgnoreCase(GameType.CREATIVE.getName()) ||
    // name.equalsIgnoreCase("c")) { 	return GameType.CREATIVE; } else if
    // (name.equalsIgnoreCase(GameType.ADVENTURE.getName()) ||
    // name.equalsIgnoreCase("a")) { 	return GameType.ADVENTURE; } else {
    // return
    // LevelSettings.validateGameType(convertArgToInt(source, name, 0,
    // GameType.values().size() - 2));
    // }
}