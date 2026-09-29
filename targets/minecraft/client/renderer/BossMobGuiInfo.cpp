#include "BossMobGuiInfo.h"

#include "minecraft/SharedConstants.h"
#include "minecraft/world/entity/boss/BossMob.h"

float BossMobGuiInfo::healthProgress = 0.0f;
int BossMobGuiInfo::displayTicks = 0;
std::string BossMobGuiInfo::name = "";
bool BossMobGuiInfo::darkenWorld = false;

void BossMobGuiInfo::setBossHealth(std::shared_ptr<BossMob> boss,
                                   bool darkenWorld) {
    healthProgress = (float)boss->getHealth() / (float)boss->getMaxHealth();
    displayTicks = SharedConstants::TICKS_PER_SECOND * 5;
    name = boss->getAName();
    BossMobGuiInfo::darkenWorld = darkenWorld;
}