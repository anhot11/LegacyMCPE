#include "ComplexItemDataPacket.h"

#include <stdint.h>
#include <string.h>

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"

ComplexItemDataPacket::~ComplexItemDataPacket() {}

ComplexItemDataPacket::ComplexItemDataPacket() {
    shouldDelay = true;
    itemType = 0;
}

ComplexItemDataPacket::ComplexItemDataPacket(short itemType, short itemId,
                                             std::vector<char>& data) {
    shouldDelay = true;
    this->itemType = itemType;
    this->itemId = itemId;
    // Take copy of array passed in as we want the packets to have full
    // ownership of any data they reference
    this->data = std::vector<char>(data.size());
    memcpy(this->data.data(), data.data(), data.size());
}

void ComplexItemDataPacket::read(DataInputStream* dis)  // throws IOException
{
    itemType = dis->readShort();
    itemId = dis->readShort();

    data = std::vector<char>(dis->readUnsignedShort() & 0xffff);
    dis->readFully(data);
}

void ComplexItemDataPacket::write(DataOutputStream* dos)  // throws IOException
{
    dos->writeShort(itemType);
    dos->writeShort(itemId);
    dos->writeUnsignedShort(data.size());

    std::vector<uint8_t> ba((uint8_t*)data.data(),
                            (uint8_t*)data.data() + data.size());
    dos->write(ba);
}

void ComplexItemDataPacket::handle(PacketListener* listener) {
    listener->handleComplexItemData(shared_from_this());
}

int ComplexItemDataPacket::getEstimatedSize() {
    return 2 + 2 + 2 + data.size();
}
