#include "AreaConstraint.h"

#include <memory>

#include "app/common/Tutorial/Constraints/TutorialConstraint.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/multiplayer/MultiPlayerLocalPlayer.h"
#include "minecraft/world/phys/AABB.h"
#include "minecraft/world/phys/Vec3.h"

AreaConstraint::AreaConstraint(int descriptionId, double x0, double y0,
                               double z0, double x1, double y1, double z1,
                               bool contains /*= true*/,
                               bool restrictsMovement /*=true*/)
    : TutorialConstraint(descriptionId) {
    messageArea = AABB(x0 + 2, y0 + 2, z0 + 2, x1 - 2, y1 - 2, z1 - 2);
    movementArea = AABB(x0, y0, z0, x1, y1, z1);

    this->contains = contains;
    m_restrictsMovement = restrictsMovement;
}

bool AreaConstraint::isConstraintSatisfied(int iPad) {
    Minecraft* minecraft = Minecraft::GetInstance();

    // TODO: check if this can be elided
    Vec3 ipad_player = minecraft->localplayers[iPad]->getPos(1);
    return messageArea.contains(ipad_player) == contains;
}

bool AreaConstraint::isConstraintRestrictive(int iPad) {
    return m_restrictsMovement;
}

bool AreaConstraint::canMoveToPosition(double xo, double yo, double zo,
                                       double xt, double yt, double zt) {
    if (!m_restrictsMovement) return true;

    Vec3 targetPos(xt, yt, zt);
    Minecraft* minecraft = Minecraft::GetInstance();

    if (movementArea.contains(targetPos) == contains) {
        return true;
    }
    Vec3 origPos(xo, yo, zo);

    double currDist = origPos.distanceTo(&movementArea);
    double targetDist = targetPos.distanceTo(&movementArea);
    return targetDist < currDist;
}
