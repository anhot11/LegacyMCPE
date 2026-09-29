#include "Abilities.h"

#include <string>

#include "nbt/CompoundTag.h"

Abilities::Abilities() {
    invulnerable = false;
    flying = false;
    mayfly = false;
    instabuild = false;
    mayBuild = true;
    flyingSpeed = 0.05f;
    walkingSpeed = 0.1f;

#ifdef _DEBUG_MENUS_ENABLED
    debugflying = false;
#endif
}

void Abilities::addSaveData(CompoundTag* parentTag) {
    CompoundTag* tag = new CompoundTag();

    tag->putBoolean("invulnerable", invulnerable);
    tag->putBoolean("flying", flying);
    tag->putBoolean("mayfly", mayfly);
    tag->putBoolean("instabuild", instabuild);
    tag->putBoolean("mayBuild", mayBuild);
    tag->putFloat("flySpeed", flyingSpeed);
    tag->putFloat("walkSpeed", walkingSpeed);

    parentTag->put("abilities", tag);
}

void Abilities::loadSaveData(CompoundTag* parentTag) {
    if (parentTag->contains("abilities")) {
        CompoundTag* tag = parentTag->getCompound("abilities");

        invulnerable = tag->getBoolean("invulnerable");
        flying = tag->getBoolean("flying");
        mayfly = tag->getBoolean("mayfly");
        instabuild = tag->getBoolean("instabuild");

        if (tag->contains("flySpeed")) {
            flyingSpeed = tag->getFloat("flySpeed");
            walkingSpeed = tag->getFloat("walkSpeed");
        }
        if (tag->contains("mayBuild")) {
            mayBuild = tag->getBoolean("mayBuild");
        }
    }
}

float Abilities::getFlyingSpeed() { return flyingSpeed; }

void Abilities::setFlyingSpeed(float value) { flyingSpeed = value; }

float Abilities::getWalkingSpeed() { return walkingSpeed; }

void Abilities::setWalkingSpeed(float value) { walkingSpeed = value; }