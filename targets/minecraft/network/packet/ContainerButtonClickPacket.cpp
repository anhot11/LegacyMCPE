#include "ContainerButtonClickPacket.h"

#include <stdint.h>

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"

ContainerButtonClickPacket::ContainerButtonClickPacket() {}

ContainerButtonClickPacket::ContainerButtonClickPacket(int containerId,
                                                       int buttonId) {
    this->containerId = containerId;
    this->buttonId = buttonId;
}

void ContainerButtonClickPacket::handle(PacketListener* listener) {
    listener->handleContainerButtonClick(shared_from_this());
}

void ContainerButtonClickPacket::read(DataInputStream* dis) {
    containerId = (int)dis->readByte();
    buttonId = (int)dis->readByte();
}

void ContainerButtonClickPacket::write(DataOutputStream* dos) {
    dos->writeByte((uint8_t)containerId);
    dos->writeByte((uint8_t)buttonId);
}

int ContainerButtonClickPacket::getEstimatedSize() { return 2; }
