#include "ContainerSetContentPacket.h"

#include <stdint.h>

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"
#include "minecraft/world/item/ItemInstance.h"

ContainerSetContentPacket::~ContainerSetContentPacket() {}

ContainerSetContentPacket::ContainerSetContentPacket() { containerId = 0; }

ContainerSetContentPacket::ContainerSetContentPacket(
    int containerId, std::vector<std::shared_ptr<ItemInstance>>* newItems) {
    this->containerId = containerId;
    items = std::vector<std::shared_ptr<ItemInstance>>((int)newItems->size());
    for (unsigned int i = 0; i < items.size(); i++) {
        std::shared_ptr<ItemInstance> item = newItems->at(i);
        items[i] = item == nullptr ? nullptr : item->copy();
    }
}

void ContainerSetContentPacket::read(
    DataInputStream* dis)  // throws IOException
{
    containerId = (int)dis->readByte();
    int count = dis->readShort();
    items = std::vector<std::shared_ptr<ItemInstance>>(count);
    for (int i = 0; i < count; i++) {
        items[i] = readItem(dis);
    }
}

void ContainerSetContentPacket::write(
    DataOutputStream* dos)  // throws IOException
{
    dos->writeByte((uint8_t)containerId);
    dos->writeShort(items.size());
    for (unsigned int i = 0; i < items.size(); i++) {
        writeItem(items[i], dos);
    }
}

void ContainerSetContentPacket::handle(PacketListener* listener) {
    listener->handleContainerContent(shared_from_this());
}

int ContainerSetContentPacket::getEstimatedSize() {
    return 3 + items.size() * 5;
}
