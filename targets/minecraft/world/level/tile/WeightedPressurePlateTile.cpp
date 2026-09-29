#include "WeightedPressurePlateTile.h"

#include <algorithm>
#include <vector>

#include "minecraft/SharedConstants.h"
#include "minecraft/util/Mth.h"
#include "minecraft/world/entity/Entity.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/redstone/Redstone.h"
#include "minecraft/world/level/tile/BasePressurePlateTile.h"
#include "minecraft/world/phys/AABB.h"

class Material;

WeightedPressurePlateTile::WeightedPressurePlateTile(int id,
                                                     const std::string& tex,
                                                     Material* material,
                                                     int maxWeight)
    : BasePressurePlateTile(id, tex, material) {
    this->maxWeight = maxWeight;

    // 4J Stu - Move this from base class to use virtual function
    updateShape(getDataForSignal(Redstone::SIGNAL_MAX));
}

int WeightedPressurePlateTile::getSignalStrength(Level* level, int x, int y,
                                                 int z) {
    AABB at_bb = getSensitiveAABB(x, y, z);
    int weightOfEntities =
        level->getEntitiesOfClass(typeid(Entity), &at_bb)->size();
    int count = std::min(weightOfEntities, maxWeight);

    if (count <= 0) {
        return 0;
    } else {
        float pct = std::min(maxWeight, count) / (float)maxWeight;
        return Mth::ceil(pct * Redstone::SIGNAL_MAX);
    }
}

int WeightedPressurePlateTile::getSignalForData(int data) { return data; }

int WeightedPressurePlateTile::getDataForSignal(int signal) { return signal; }

int WeightedPressurePlateTile::getTickDelay(Level* level) {
    return SharedConstants::TICKS_PER_SECOND / 2;
}
