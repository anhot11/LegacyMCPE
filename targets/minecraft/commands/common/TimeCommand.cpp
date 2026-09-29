#include "TimeCommand.h"

#include <string>
#include <vector>

#include "java/InputOutputStream/ByteArrayInputStream.h"
#include "java/InputOutputStream/ByteArrayOutputStream.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"
#include "minecraft/commands/CommandsEnum.h"
#include "minecraft/network/packet/ChatPacket.h"
#include "minecraft/network/packet/GameCommandPacket.h"
#include "minecraft/server/MinecraftServer.h"
#include "minecraft/server/level/ServerLevel.h"

class CommandSender;

EGameCommand TimeCommand::getId() { return eGameCommand_Time; }

int TimeCommand::getPermissionLevel() { return LEVEL_GAMEMASTERS; }

void TimeCommand::execute(std::shared_ptr<CommandSender> source,
                          std::vector<uint8_t>& commandData) {
    ByteArrayInputStream bais(commandData);
    DataInputStream dis(&bais);

    bool night = dis.readBoolean();

    bais.reset();

    int amount = 0;
    if (night) amount = 12500;
    doSetTime(source, amount);
    // logAdminAction(source, "commands.time.set", amount);
    logAdminAction(source, ChatPacket::e_ChatCustom, "commands.time.set");

    // if (args.size() > 1) {
    //	if (args[0].equals("set")) {
    //		int amount;

    //		if (args[1].equals("day")) {
    //			amount = 0;
    //		} else if (args[1].equals("night")) {
    //			amount = 12500;
    //		} else {
    //			amount = convertArgToInt(source, args[1], 0);
    //		}

    //		doSetTime(source, amount);
    //		logAdminAction(source, "commands.time.set", amount);
    //		return;
    //	} else if (args[0].equals("add")) {
    //		int amount = convertArgToInt(source, args[1], 0);
    //		doAddTime(source, amount);

    //		logAdminAction(source, "commands.time.added", amount);
    //		return;
    //	}
    //}

    // throw new UsageException("commands.time.usage");
}

void TimeCommand::doSetTime(std::shared_ptr<CommandSender> source, int value) {
    for (int i = 0; i < MinecraftServer::getInstance()->levels.size(); i++) {
        MinecraftServer::getInstance()->levels[i]->setDayTime(value);
    }
}

void TimeCommand::doAddTime(std::shared_ptr<CommandSender> source, int value) {
    for (int i = 0; i < MinecraftServer::getInstance()->levels.size(); i++) {
        ServerLevel* level = MinecraftServer::getInstance()->levels[i];
        level->setDayTime(level->getDayTime() + value);
    }
}

std::shared_ptr<GameCommandPacket> TimeCommand::preparePacket(bool night) {
    ByteArrayOutputStream baos;
    DataOutputStream dos(&baos);

    dos.writeBoolean(night);

    return std::shared_ptr<GameCommandPacket>(
        new GameCommandPacket(eGameCommand_Time, baos.toByteArray()));
}