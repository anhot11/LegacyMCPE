#include "BegGoal.h"

#include "java/Random.h"
#include "minecraft/world/entity/ai/control/Control.h"
#include "minecraft/world/entity/ai/control/LookControl.h"
#include "minecraft/world/entity/animal/Wolf.h"
#include "minecraft/world/entity/player/Inventory.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/Level.h"

BegGoal::BegGoal(Wolf* wolf, float lookDistance) {
    player = std::weak_ptr<Player>();
    lookTime = 0;

    this->wolf = wolf;
    this->level = wolf->level;
    this->lookDistance = lookDistance;
    setRequiredControlFlags(Control::LookControlFlag);
}

bool BegGoal::canUse() {
    player = std::weak_ptr<Player>(
        level->getNearestPlayer(wolf->shared_from_this(), lookDistance));
    if (player.lock() == nullptr) return false;
    wolf->setDespawnProtected();
    return playerHoldingInteresting(player.lock());
}

bool BegGoal::canContinueToUse() {
    if (player.lock() == nullptr || !player.lock()->isAlive()) return false;
    if (wolf->distanceToSqr(player.lock()) > lookDistance * lookDistance)
        return false;
    wolf->setDespawnProtected();
    return lookTime > 0 && playerHoldingInteresting(player.lock());
}

void BegGoal::start() {
    wolf->setIsInterested(true);
    lookTime = 40 + wolf->getRandom()->nextInt(40);
}

void BegGoal::stop() {
    wolf->setIsInterested(false);
    player = std::weak_ptr<Player>();
}

void BegGoal::tick() {
    wolf->getLookControl()->setLookAt(
        player.lock()->x, player.lock()->y + player.lock()->getHeadHeight(),
        player.lock()->z, 10, wolf->getMaxHeadXRot());
    --lookTime;
}

bool BegGoal::playerHoldingInteresting(std::shared_ptr<Player> player) {
    std::shared_ptr<ItemInstance> item = player->inventory->getSelected();
    if (item == nullptr) return false;
    if (!wolf->isTame() && item->id == Item::bone_Id) return true;
    return wolf->isFood(item);
}
