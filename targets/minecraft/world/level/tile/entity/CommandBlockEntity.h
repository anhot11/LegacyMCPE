#pragma once

#include <string>

#include "TileEntity.h"
#include "java/Class.h"
#include "minecraft/commands/CommandSender.h"
#include "minecraft/commands/CommandsEnum.h"
#include "minecraft/network/packet/ChatPacket.h"

class ChatMessageComponent;
class Level;
class Pos;

class CommandBlockEntity : public TileEntity, public CommandSender {
public:
    eINSTANCEOF GetType() { return eTYPE_COMMANDBLOCKTILEENTITY; }
    static TileEntity* create() { return new CommandBlockEntity(); }

    // 4J Added
    virtual std::shared_ptr<TileEntity> clone();

private:
    int successCount;
    std::string command;
    std::string name;

public:
    CommandBlockEntity();

    void setCommand(const std::string& command);
    std::string getCommand();
    int performCommand(Level* level);
    std::string getName();
    void setName(const std::string& name);
    virtual void sendMessage(
        const std::string& message,
        ChatPacket::EChatPacketMessage type = ChatPacket::e_ChatCustom,
        int customData = -1, const std::string& additionalMessage = "");
    virtual bool hasPermission(EGameCommand command);
    // void sendMessage(ChatMessageComponent *message);
    // bool hasPermission(int permissionLevel, const std::string &commandName);
    void save(CompoundTag* tag);
    void load(CompoundTag* tag);
    Pos* getCommandSenderWorldPosition();
    Level* getCommandSenderWorld();
    std::shared_ptr<Packet> getUpdatePacket();
    int getSuccessCount();
    void setSuccessCount(int successCount);
};