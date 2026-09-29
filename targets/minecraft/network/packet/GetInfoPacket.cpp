#include "GetInfoPacket.h"

#include "PacketListener.h"

void GetInfoPacket::read(DataInputStream* dis) {}

void GetInfoPacket::write(DataOutputStream* dos) {}

void GetInfoPacket::handle(PacketListener* listener) {
    listener->handleGetInfo(shared_from_this());
}

int GetInfoPacket::getEstimatedSize() { return 0; }