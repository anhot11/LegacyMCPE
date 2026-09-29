#include "PlayerCommandPacket.h"

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"
#include "minecraft/world/entity/Entity.h"

const int PlayerCommandPacket::START_SNEAKING = 1;
const int PlayerCommandPacket::STOP_SNEAKING = 2;
const int PlayerCommandPacket::STOP_SLEEPING = 3;
const int PlayerCommandPacket::START_SPRINTING = 4;
const int PlayerCommandPacket::STOP_SPRINTING = 5;
const int PlayerCommandPacket::START_IDLEANIM = 6;
const int PlayerCommandPacket::STOP_IDLEANIM = 7;
const int PlayerCommandPacket::RIDING_JUMP = 8;
const int PlayerCommandPacket::OPEN_INVENTORY = 9;

PlayerCommandPacket::PlayerCommandPacket() {
    id = -1;
    action = 0;
    data = 0;
}

PlayerCommandPacket::PlayerCommandPacket(std::shared_ptr<Entity> e,
                                         int action) {
    id = e->entityId;
    this->action = action;
    this->data = 0;
}

PlayerCommandPacket::PlayerCommandPacket(std::shared_ptr<Entity> e, int action,
                                         int data) {
    id = e->entityId;
    this->action = action;
    this->data = data;
}

void PlayerCommandPacket::read(DataInputStream* dis)  // throws IOException
{
    id = dis->readInt();
    action = dis->readByte();
    data = dis->readInt();
}

void PlayerCommandPacket::write(DataOutputStream* dos)  // throws IOException
{
    dos->writeInt(id);
    dos->writeByte(action);
    dos->writeInt(data);
}

void PlayerCommandPacket::handle(PacketListener* listener) {
    listener->handlePlayerCommand(shared_from_this());
}

int PlayerCommandPacket::getEstimatedSize() { return 9; }
