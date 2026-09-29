#include "RemoveMobEffectPacket.h"

#include <stdint.h>

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"
#include "minecraft/world/effect/MobEffectInstance.h"

RemoveMobEffectPacket::RemoveMobEffectPacket() {}

RemoveMobEffectPacket::RemoveMobEffectPacket(int entityId,
                                             MobEffectInstance* effect) {
    this->entityId = entityId;
    this->effectId = (uint8_t)(effect->getId() & 0xff);
}

void RemoveMobEffectPacket::read(DataInputStream* dis) {
    entityId = dis->readInt();
    effectId = dis->readByte();
}

void RemoveMobEffectPacket::write(DataOutputStream* dos) {
    dos->writeInt(entityId);
    dos->writeByte(effectId);
}

void RemoveMobEffectPacket::handle(PacketListener* listener) {
    listener->handleRemoveMobEffect(shared_from_this());
}

int RemoveMobEffectPacket::getEstimatedSize() { return 5; }