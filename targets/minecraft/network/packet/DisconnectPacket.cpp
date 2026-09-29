#include "minecraft/network/packet/DisconnectPacket.h"

#include <stdio.h>

#include <memory>

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"

DisconnectPacket::DisconnectPacket() { reason = eDisconnect_None; }

DisconnectPacket::DisconnectPacket(eDisconnectReason reason) {
    this->reason = reason;
}

void DisconnectPacket::read(DataInputStream* dis)  // throws IOException
{
    reason = (eDisconnectReason)dis->readInt();
    fprintf(stderr, "[PKT] DisconnectPacket::read reason=%d\n", reason);
}

void DisconnectPacket::write(DataOutputStream* dos)  // throws IOException
{
    fprintf(stderr, "[PKT] DisconnectPacket::write reason=%d\n", reason);
    dos->writeInt((int)reason);
}

void DisconnectPacket::handle(PacketListener* listener) {
    listener->handleDisconnect(shared_from_this());
}

int DisconnectPacket::getEstimatedSize() { return sizeof(eDisconnectReason); }

bool DisconnectPacket::canBeInvalidated() { return true; }

bool DisconnectPacket::isInvalidatedBy(std::shared_ptr<Packet> packet) {
    return true;
}