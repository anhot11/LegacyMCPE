#include "UpdateMobEffectPacket.h"

#include <limits>

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"
#include "minecraft/network/packet/Packet.h"
#include "minecraft/world/effect/MobEffectInstance.h"

UpdateMobEffectPacket::UpdateMobEffectPacket() {
    entityId = 0;
    effectId = 0;
    effectAmplifier = 0;
    effectDurationTicks = 0;
}

UpdateMobEffectPacket::UpdateMobEffectPacket(int entityId,
                                             MobEffectInstance* effect) {
    this->entityId = entityId;
    effectId = (uint8_t)(effect->getId() & 0xff);
    effectAmplifier = (char)(effect->getAmplifier() & 0xff);

    if (effect->getDuration() > std::numeric_limits<short>::max()) {
        effectDurationTicks = std::numeric_limits<short>::max();
    } else {
        effectDurationTicks = (short)effect->getDuration();
    }
}

void UpdateMobEffectPacket::read(DataInputStream* dis) {
    entityId = dis->readInt();
    effectId = dis->readByte();
    effectAmplifier = dis->readByte();
    effectDurationTicks = dis->readShort();
}

void UpdateMobEffectPacket::write(DataOutputStream* dos) {
    dos->writeInt(entityId);
    dos->writeByte(effectId);
    dos->writeByte(effectAmplifier);
    dos->writeShort(effectDurationTicks);
}

bool UpdateMobEffectPacket::isSuperLongDuration() {
    return effectDurationTicks == std::numeric_limits<short>::max();
}

void UpdateMobEffectPacket::handle(PacketListener* listener) {
    listener->handleUpdateMobEffect(shared_from_this());
}

int UpdateMobEffectPacket::getEstimatedSize() { return 8; }

bool UpdateMobEffectPacket::canBeInvalidated() { return true; }

bool UpdateMobEffectPacket::isInvalidatedBy(std::shared_ptr<Packet> packet) {
    std::shared_ptr<UpdateMobEffectPacket> target =
        std::dynamic_pointer_cast<UpdateMobEffectPacket>(packet);
    return target->entityId == entityId && target->effectId == effectId;
}
