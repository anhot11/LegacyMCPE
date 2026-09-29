#include "KeepAlivePacket.h"

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"

KeepAlivePacket::KeepAlivePacket() { id = 0; }

KeepAlivePacket::KeepAlivePacket(int id) { this->id = id; }

void KeepAlivePacket::handle(PacketListener* listener) {
    listener->handleKeepAlive(shared_from_this());
}

void KeepAlivePacket::read(DataInputStream* dis)  // throws IOException
{
    id = dis->readInt();
}

void KeepAlivePacket::write(DataOutputStream* dos)  // throws IOException
{
    dos->writeInt(id);
}

int KeepAlivePacket::getEstimatedSize() { return 4; }

bool KeepAlivePacket::canBeInvalidated() { return true; }

bool KeepAlivePacket::isInvalidatedBy(std::shared_ptr<Packet> packet) {
    return true;
}

bool KeepAlivePacket::isAync() { return true; }