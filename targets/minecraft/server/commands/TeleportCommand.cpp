#include "TeleportCommand.h"

#include <string>

#include "java/Class.h"
#include "java/InputOutputStream/ByteArrayInputStream.h"
#include "java/InputOutputStream/ByteArrayOutputStream.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"
#include "minecraft/commands/CommandsEnum.h"
#include "minecraft/network/packet/ChatPacket.h"
#include "minecraft/network/packet/GameCommandPacket.h"
#include "minecraft/server/MinecraftServer.h"
#include "minecraft/server/PlayerList.h"
#include "minecraft/server/level/ServerPlayer.h"
#include "minecraft/server/network/PlayerConnection.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/dimension/Dimension.h"

EGameCommand TeleportCommand::getId() { return eGameCommand_Teleport; }

void TeleportCommand::execute(std::shared_ptr<CommandSender> source,
                              std::vector<uint8_t>& commandData) {
    ByteArrayInputStream bais(commandData);
    DataInputStream dis(&bais);

    PlayerUID subjectID = dis.readPlayerUID();
    PlayerUID destinationID = dis.readPlayerUID();

    bais.reset();

    PlayerList* players = MinecraftServer::getInstance()->getPlayerList();

    std::shared_ptr<ServerPlayer> subject = players->getPlayer(subjectID);
    std::shared_ptr<ServerPlayer> destination =
        players->getPlayer(destinationID);

    if (subject != nullptr && destination != nullptr &&
        subject->level->dimension->id == destination->level->dimension->id &&
        subject->isAlive()) {
        subject->ride(nullptr);
        subject->connection->teleport(destination->x, destination->y,
                                      destination->z, destination->yRot,
                                      destination->xRot);
        // logAdminAction(source, "commands.tp.success", subject->getAName(),
        // destination->getAName());
        logAdminAction(source, ChatPacket::e_ChatCommandTeleportSuccess,
                       subject->getName(), eTYPE_SERVERPLAYER,
                       destination->getName());

        if (subject == source) {
            destination->sendMessage(subject->getName(),
                                     ChatPacket::e_ChatCommandTeleportToMe);
        } else {
            subject->sendMessage(destination->getName(),
                                 ChatPacket::e_ChatCommandTeleportMe);
        }
    }

    // if (args.size() >= 1) {
    //	MinecraftServer server = MinecraftServer.getInstance();
    //	ServerPlayer victim;

    //	if (args.size() == 2 || args.size() == 4) {
    //		victim = server.getPlayers().getPlayer(args[0]);
    //		if (victim == null) throw new PlayerNotFoundException();
    //	} else {
    //		victim = (ServerPlayer) convertSourceToPlayer(source);
    //	}

    //	if (args.size() == 3 || args.size() == 4) {
    //		if (victim.level != null) {
    //			int pos = args.size() - 3;
    //			int maxPos = Level.MAX_LEVEL_SIZE;
    //			int x = convertArgToInt(source, args[pos++], -maxPos,
    // maxPos); 			int y = convertArgToInt(source,
    // args[pos++], Level.minBuildHeight, Level.maxBuildHeight);
    // int z = convertArgToInt(source, args[pos++], -maxPos, maxPos);

    //			victim.teleportTo(x + 0.5f, y, z + 0.5f);
    //			logAdminAction(source, "commands.tp.coordinates",
    // victim.getAName(), x, y, z);
    //		}
    //	} else if (args.size() == 1 || args.size() == 2) {
    //		ServerPlayer destination =
    // server.getPlayers().getPlayer(args[args.size() - 1]); 		if
    // (destination == null) throw new PlayerNotFoundException();

    //		victim.connection.teleport(destination.x, destination.y,
    // destination.z, destination.yRot, destination.xRot);
    // logAdminAction(source, "commands.tp.success", victim.getAName(),
    // destination.getAName());
    //	}
    //}
}

std::shared_ptr<GameCommandPacket> TeleportCommand::preparePacket(
    PlayerUID subject, PlayerUID destination) {
    ByteArrayOutputStream baos;
    DataOutputStream dos(&baos);

    dos.writePlayerUID(subject);
    dos.writePlayerUID(destination);

    return std::shared_ptr<GameCommandPacket>(
        new GameCommandPacket(eGameCommand_Teleport, baos.toByteArray()));
}