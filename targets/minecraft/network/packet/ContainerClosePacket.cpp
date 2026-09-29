#include "ContainerClosePacket.h"

#include <stdint.h>

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"

ContainerClosePacket::ContainerClosePacket() { containerId = 0; }

ContainerClosePacket::ContainerClosePacket(int containerId) {
    this->containerId = containerId;
}

void ContainerClosePacket::handle(PacketListener* listener) {
    listener->handleContainerClose(shared_from_this());
}

void ContainerClosePacket::read(DataInputStream* dis)  // throws IOException
{
    containerId = (int)dis->readByte();
}

void ContainerClosePacket::write(DataOutputStream* dos)  // throws IOException
{
    dos->writeByte((uint8_t)containerId);
}

int ContainerClosePacket::getEstimatedSize() { return 1; }
