#include "SetEntityLinkPacket.h"

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"
#include "minecraft/network/packet/Packet.h"
#include "minecraft/world/entity/Entity.h"

SetEntityLinkPacket::SetEntityLinkPacket() {
    sourceId = -1;
    destId = -1;
    type = -1;
}

SetEntityLinkPacket::SetEntityLinkPacket(int linkType,
                                         std::shared_ptr<Entity> sourceEntity,
                                         std::shared_ptr<Entity> destEntity) {
    type = linkType;
    this->sourceId = sourceEntity->entityId;
    this->destId = destEntity != nullptr ? destEntity->entityId : -1;
}

int SetEntityLinkPacket::getEstimatedSize() { return 8; }

void SetEntityLinkPacket::read(DataInputStream* dis)  // throws IOException
{
    sourceId = dis->readInt();
    destId = dis->readInt();
    type = dis->readUnsignedByte();
}

void SetEntityLinkPacket::write(DataOutputStream* dos)  // throws IOException
{
    dos->writeInt(sourceId);
    dos->writeInt(destId);
    dos->writeByte(type);
}

void SetEntityLinkPacket::handle(PacketListener* listener) {
    listener->handleEntityLinkPacket(shared_from_this());
}

bool SetEntityLinkPacket::canBeInvalidated() { return true; }

bool SetEntityLinkPacket::isInvalidatedBy(std::shared_ptr<Packet> packet) {
    std::shared_ptr<SetEntityLinkPacket> target =
        std::dynamic_pointer_cast<SetEntityLinkPacket>(packet);
    return target->sourceId == sourceId;
}
