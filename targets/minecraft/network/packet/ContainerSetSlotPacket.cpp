#include "ContainerSetSlotPacket.h"

#include <stdint.h>

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"
#include "minecraft/world/item/ItemInstance.h"

const int ContainerSetSlotPacket::CONTAINER = 0;
const int ContainerSetSlotPacket::WORKBENCH = 1;
const int ContainerSetSlotPacket::FURNACE = 2;

ContainerSetSlotPacket::ContainerSetSlotPacket() {
    containerId = 0;
    slot = 0;
    item = nullptr;
}

ContainerSetSlotPacket::ContainerSetSlotPacket(
    int containerId, int slot, std::shared_ptr<ItemInstance> item) {
    this->containerId = containerId;
    this->slot = slot;
    this->item = item == nullptr ? item : item->copy();
}

void ContainerSetSlotPacket::handle(PacketListener* listener) {
    listener->handleContainerSetSlot(shared_from_this());
}

void ContainerSetSlotPacket::read(DataInputStream* dis)  // throws IOException
{
    // 4J Stu - TU-1 hotfix
    // Fix for #13142 - Holding down the A button on the furnace ingredient slot
    // causes the UI to display incorrect item counts
    uint8_t byteId = dis->readByte();
    containerId = *(char*)&byteId;
    slot = dis->readShort();
    item = readItem(dis);
}

void ContainerSetSlotPacket::write(DataOutputStream* dos)  // throws IOException
{
    dos->writeByte((uint8_t)containerId);
    dos->writeShort(slot);
    writeItem(item, dos);
}

int ContainerSetSlotPacket::getEstimatedSize() { return 3 + 5; }
