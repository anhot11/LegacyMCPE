#include "SetHealthPacket.h"

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"

SetHealthPacket::SetHealthPacket() {
    this->health = 0.0f;
    this->food = 0;
    this->saturation = 0;

    this->damageSource = 0;  // 4jcraft: previously eTelemetryChallenges_Unknown
}

SetHealthPacket::SetHealthPacket(float health, int food, float saturation,
                                 uint8_t damageSource) {
    this->health = health;
    this->food = food;
    this->saturation = saturation;
    // this.exhaustion = exhaustion; // 4J - Original comment

    this->damageSource = damageSource;
}

void SetHealthPacket::read(DataInputStream* dis)  // throws IOException
{
    health = dis->readFloat();
    food = dis->readShort();
    saturation = dis->readFloat();

    damageSource = (uint8_t)dis->readByte();
}

void SetHealthPacket::write(DataOutputStream* dos)  // throws IOException
{
    dos->writeFloat(health);
    dos->writeShort(food);
    dos->writeFloat(saturation);

    dos->writeByte(damageSource);
}

void SetHealthPacket::handle(PacketListener* listener) {
    listener->handleSetHealth(shared_from_this());
}

int SetHealthPacket::getEstimatedSize() { return 11; }

bool SetHealthPacket::canBeInvalidated() { return true; }

bool SetHealthPacket::isInvalidatedBy(std::shared_ptr<Packet> packet) {
    return true;
}