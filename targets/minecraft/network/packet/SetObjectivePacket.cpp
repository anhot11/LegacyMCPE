#include "SetObjectivePacket.h"

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"
#include "minecraft/world/scores/Objective.h"

SetObjectivePacket::SetObjectivePacket() {
    objectiveName = "";
    displayName = "";
    method = 0;
}

SetObjectivePacket::SetObjectivePacket(Objective* objective, int method) {
    objectiveName = objective->getName();
    displayName = objective->getDisplayName();
    this->method = method;
}

void SetObjectivePacket::read(DataInputStream* dis) {
    objectiveName = readUtf(dis, Objective::MAX_NAME_LENGTH);
    displayName = readUtf(dis, Objective::MAX_DISPLAY_NAME_LENGTH);
    method = dis->readByte();
}

void SetObjectivePacket::write(DataOutputStream* dos) {
    writeUtf(objectiveName, dos);
    writeUtf(displayName, dos);
    dos->writeByte(method);
}

void SetObjectivePacket::handle(PacketListener* listener) {
    listener->handleAddObjective(shared_from_this());
}

int SetObjectivePacket::getEstimatedSize() {
    return 2 + objectiveName.length() + 2 + displayName.length() + 1;
}