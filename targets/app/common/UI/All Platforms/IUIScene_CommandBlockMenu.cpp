#include "IUIScene_CommandBlockMenu.h"

#include <memory>

#include "java/InputOutputStream/ByteArrayOutputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/multiplayer/ClientConnection.h"
#include "minecraft/client/multiplayer/MultiPlayerLocalPlayer.h"
#include "minecraft/network/packet/CustomPayloadPacket.h"
#include "minecraft/world/level/tile/entity/CommandBlockEntity.h"

void IUIScene_CommandBlockMenu::Initialise(CommandBlockEntity* commandBlock) {
    m_commandBlock = commandBlock;
    SetCommand(m_commandBlock->getCommand());
}

void IUIScene_CommandBlockMenu::ConfirmButtonClicked() {
    ByteArrayOutputStream baos;
    DataOutputStream dos(&baos);

    dos.writeInt(m_commandBlock->x);
    dos.writeInt(m_commandBlock->y);
    dos.writeInt(m_commandBlock->z);
    dos.writeUTF(GetCommand());

    Minecraft::GetInstance()->localplayers[GetPad()]->connection->send(
        std::shared_ptr<CustomPayloadPacket>(new CustomPayloadPacket(
            CustomPayloadPacket::SET_ADVENTURE_COMMAND_PACKET,
            baos.toByteArray())));
}
