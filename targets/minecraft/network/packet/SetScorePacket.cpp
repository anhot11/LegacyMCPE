#include "SetScorePacket.h"

#include "PacketListener.h"
#include "java/InputOutputStream/DataInputStream.h"
#include "java/InputOutputStream/DataOutputStream.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/scores/Objective.h"
#include "minecraft/world/scores/Score.h"

SetScorePacket::SetScorePacket() {
    owner = "";
    objectiveName = "";
    score = 0;
    method = 0;
}

SetScorePacket::SetScorePacket(Score* score, int method) {
    owner = score->getOwner();
    objectiveName = score->getObjective()->getName();
    this->score = score->getScore();
    this->method = method;
}

SetScorePacket::SetScorePacket(const std::string& owner) {
    this->owner = owner;
    objectiveName = "";
    score = 0;
    method = METHOD_REMOVE;
}

void SetScorePacket::read(DataInputStream* dis) {
    owner = readUtf(dis, Player::MAX_NAME_LENGTH);
    method = dis->readByte();

    if (method != METHOD_REMOVE) {
        objectiveName = readUtf(dis, Objective::MAX_NAME_LENGTH);
        score = dis->readInt();
    }
}

void SetScorePacket::write(DataOutputStream* dos) {
    writeUtf(owner, dos);
    dos->writeByte(method);

    if (method != METHOD_REMOVE) {
        writeUtf(objectiveName, dos);
        dos->writeInt(score);
    }
}

void SetScorePacket::handle(PacketListener* listener) {
    listener->handleSetScore(shared_from_this());
}

int SetScorePacket::getEstimatedSize() {
    return 2 + (owner.empty() ? 0 : owner.length()) + 2 +
           (objectiveName.empty() ? 0 : objectiveName.length()) + 4 + 1;
}