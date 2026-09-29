#include "ClientCommandPacket.h"

#include <stdint.h>

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"

ClientCommandPacket::ClientCommandPacket() { action = 0; }

ClientCommandPacket::ClientCommandPacket(int action) { this->action = action; }

void ClientCommandPacket::read(DataInputStream* dis) {
    action = (int)dis->readByte();
}

void ClientCommandPacket::write(DataOutputStream* dos) {
    dos->writeByte((uint8_t)action & (uint8_t)0xff);
}

void ClientCommandPacket::handle(PacketListener* listener) {
    listener->handleClientCommand(
        std::dynamic_pointer_cast<ClientCommandPacket>(shared_from_this()));
}

int ClientCommandPacket::getEstimatedSize() { return 1; }
