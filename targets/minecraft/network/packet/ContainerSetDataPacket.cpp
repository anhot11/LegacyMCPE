#include "ContainerSetDataPacket.h"

#include <stdint.h>

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"

ContainerSetDataPacket::ContainerSetDataPacket() {
    containerId = 0;
    id = -1;
    value = 0;
}

ContainerSetDataPacket::ContainerSetDataPacket(int containerId, int id,
                                               int value) {
    this->containerId = containerId;
    this->id = id;
    this->value = value;
}

void ContainerSetDataPacket::handle(PacketListener* listener) {
    listener->handleContainerSetData(shared_from_this());
}

void ContainerSetDataPacket::read(DataInputStream* dis)  // throws IOException
{
    containerId = (int)dis->readByte();
    id = dis->readShort();
    value = dis->readShort();
}

void ContainerSetDataPacket::write(DataOutputStream* dos)  // throws IOException
{
    dos->writeByte((uint8_t)containerId);
    dos->writeShort(id);
    dos->writeShort(value);
}

int ContainerSetDataPacket::getEstimatedSize() { return 1 + 4; }
