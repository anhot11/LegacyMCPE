#include "RotateHeadPacket.h"

#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"
#include "minecraft/network/packet/Packet.h"
#include "minecraft/network/packet/PacketListener.h"

RotateHeadPacket::RotateHeadPacket() {}

RotateHeadPacket::RotateHeadPacket(int id, char yHeadRot) {
    this->id = id;
    this->yHeadRot = yHeadRot;
}

void RotateHeadPacket::read(DataInputStream* dis) {
    id = dis->readInt();
    yHeadRot = dis->readByte();
}

void RotateHeadPacket::write(DataOutputStream* dos) {
    dos->writeInt(id);
    dos->writeByte(yHeadRot);
}

void RotateHeadPacket::handle(PacketListener* listener) {
    listener->handleRotateMob(shared_from_this());
}

int RotateHeadPacket::getEstimatedSize() { return 5; }

bool RotateHeadPacket::canBeInvalidated() { return true; }

bool RotateHeadPacket::isInvalidatedBy(std::shared_ptr<Packet> packet) {
    std::shared_ptr<RotateHeadPacket> target =
        std::dynamic_pointer_cast<RotateHeadPacket>(packet);
    return target->id == id;
}

bool RotateHeadPacket::isAync() { return true; }