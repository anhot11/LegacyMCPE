#include "SetCreativeModeSlotPacket.h"

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"
#include "minecraft/world/item/ItemInstance.h"

SetCreativeModeSlotPacket::SetCreativeModeSlotPacket() {
    this->slotNum = 0;
    this->item = nullptr;
}

SetCreativeModeSlotPacket::SetCreativeModeSlotPacket(
    int slotNum, std::shared_ptr<ItemInstance> item) {
    this->slotNum = slotNum;
    // 4J - take copy of item as we want our packets to have full ownership of
    // any referenced data
    this->item = item ? item->copy() : std::shared_ptr<ItemInstance>();
}

void SetCreativeModeSlotPacket::handle(PacketListener* listener) {
    listener->handleSetCreativeModeSlot(shared_from_this());
}

void SetCreativeModeSlotPacket::read(DataInputStream* dis) {
    slotNum = dis->readShort();
    item = readItem(dis);
}

void SetCreativeModeSlotPacket::write(DataOutputStream* dos) {
    dos->writeShort(slotNum);
    writeItem(item, dos);
}

int SetCreativeModeSlotPacket::getEstimatedSize() { return 8; }