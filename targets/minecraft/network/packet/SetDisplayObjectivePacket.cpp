#include "SetDisplayObjectivePacket.h"

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"
#include "minecraft/world/scores/Objective.h"

SetDisplayObjectivePacket::SetDisplayObjectivePacket() {
    slot = 0;
    objectiveName = "";
}

SetDisplayObjectivePacket::SetDisplayObjectivePacket(int slot,
                                                     Objective* objective) {
    this->slot = slot;

    if (objective == nullptr) {
        objectiveName = "";
    } else {
        objectiveName = objective->getName();
    }
}

void SetDisplayObjectivePacket::read(DataInputStream* dis) {
    slot = dis->readByte();
    objectiveName = readUtf(dis, Objective::MAX_NAME_LENGTH);
}

void SetDisplayObjectivePacket::write(DataOutputStream* dos) {
    dos->writeByte(slot);
    writeUtf(objectiveName, dos);
}

void SetDisplayObjectivePacket::handle(PacketListener* listener) {
    listener->handleSetDisplayObjective(shared_from_this());
}

int SetDisplayObjectivePacket::getEstimatedSize() {
    return 1 + 2 + objectiveName.length();
}