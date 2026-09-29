#include "SetCarriedItemPacket.h"

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"

SetCarriedItemPacket::SetCarriedItemPacket() { slot = 0; }

SetCarriedItemPacket::SetCarriedItemPacket(int slot) { this->slot = slot; }

void SetCarriedItemPacket::read(DataInputStream* dis)  // throws IOException
{
    slot = dis->readShort();
}

void SetCarriedItemPacket::write(DataOutputStream* dos)  // throws IOException
{
    dos->writeShort(slot);
}

void SetCarriedItemPacket::handle(PacketListener* listener) {
    listener->handleSetCarriedItem(shared_from_this());
}

int SetCarriedItemPacket::getEstimatedSize() { return 2; }

bool SetCarriedItemPacket::canBeInvalidated() { return true; }

bool SetCarriedItemPacket::isInvalidatedBy(std::shared_ptr<Packet> packet) {
    return true;
}