#include "RecordingItem.h"

#include <wchar.h>

#include <memory>
#include <utility>
#include <vector>

#include "minecraft/GameEnums.h"
#include "minecraft/stats/GenericStats.h"
#include "minecraft/util/HtmlString.h"
#include "minecraft/world/IconRegister.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/item/Rarity.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/tile/JukeboxTile.h"
#include "minecraft/world/level/tile/LevelEvent.h"
#include "minecraft/world/level/tile/Tile.h"

std::unordered_map<std::string, RecordingItem*> RecordingItem::BY_NAME;

RecordingItem::RecordingItem(int id, const std::string& recording)
    : Item(id), recording(recording) {
    this->maxStackSize = 1;
    BY_NAME[recording] = this;
}

Icon* RecordingItem::getIcon(int auxValue) { return icon; }

bool RecordingItem::useOn(std::shared_ptr<ItemInstance> itemInstance,
                          std::shared_ptr<Player> player, Level* level, int x,
                          int y, int z, int face, float clickX, float clickY,
                          float clickZ, bool bTestUseOnOnly) {
    // 4J-PB - Adding a test only version to allow tooltips to be displayed
    if (level->getTile(x, y, z) == Tile::jukebox_Id &&
        level->getData(x, y, z) == 0) {
        if (!bTestUseOnOnly) {
            if (level->isClientSide) return true;

            ((JukeboxTile*)Tile::jukebox)
                ->setRecord(level, x, y, z, itemInstance);
            level->levelEvent(nullptr, LevelEvent::SOUND_PLAY_RECORDING, x, y,
                              z, id);
            itemInstance->count--;

            player->awardStat(GenericStats::musicToMyEars(),
                              GenericStats::param_musicToMyEars(id));
        }
        return true;
    }
    return false;
}

void RecordingItem::appendHoverText(std::shared_ptr<ItemInstance> itemInstance,
                                    std::shared_ptr<Player> player,
                                    std::vector<HtmlString>* lines,
                                    bool advanced) {
    eMinecraftColour color = getRarity(std::shared_ptr<ItemInstance>())->color;

    char text[256];
    snprintf(text, 256, "%s %s", "C418 -", recording.c_str());

    lines->push_back(HtmlString(text, color));
}

const Rarity* RecordingItem::getRarity(
    std::shared_ptr<ItemInstance> itemInstance) {
    return (Rarity*)Rarity::rare;
}

void RecordingItem::registerIcons(IconRegister* iconRegister) {
    icon = iconRegister->registerIcon("record_" + recording);
}

RecordingItem* RecordingItem::getByName(const std::string& name) {
    auto it = BY_NAME.find(name);
    if (it != BY_NAME.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}