#include "JukeboxTile.h"

#include <string>

#include "LevelEvent.h"
#include "java/Random.h"
#include "minecraft/Facing.h"
#include "minecraft/world/IconRegister.h"
#include "minecraft/world/entity/item/ItemEntity.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/redstone/Redstone.h"
#include "minecraft/world/level/tile/BaseEntityTile.h"
#include "minecraft/world/level/tile/Tile.h"
#include "minecraft/world/level/tile/entity/TileEntity.h"
#include "nbt/CompoundTag.h"

JukeboxTile::Entity::Entity() : TileEntity() { record = nullptr; }

void JukeboxTile::Entity::load(CompoundTag* tag) {
    TileEntity::load(tag);

    if (tag->contains("RecordItem")) {
        setRecord(ItemInstance::fromTag(tag->getCompound("RecordItem")));
    } else if (tag->getInt("Record") > 0) {
        setRecord(std::shared_ptr<ItemInstance>(
            new ItemInstance(tag->getInt("Record"), 1, 0)));
    }
}

void JukeboxTile::Entity::save(CompoundTag* tag) {
    TileEntity::save(tag);

    if (getRecord() != nullptr) {
        tag->putCompound("RecordItem", getRecord()->save(new CompoundTag()));

        tag->putInt("Record", getRecord()->id);
    }
}

// 4J Added
std::shared_ptr<TileEntity> JukeboxTile::Entity::clone() {
    std::shared_ptr<JukeboxTile::Entity> result =
        std::make_shared<JukeboxTile::Entity>();
    TileEntity::clone(result);

    result->record = record;

    return result;
}

std::shared_ptr<ItemInstance> JukeboxTile::Entity::getRecord() {
    return record;
}

void JukeboxTile::Entity::setRecord(std::shared_ptr<ItemInstance> record) {
    this->record = record;
    setChanged();
}

JukeboxTile::JukeboxTile(int id) : BaseEntityTile(id, Material::wood) {
    iconTop = nullptr;
}

Icon* JukeboxTile::getTexture(int face, int data) {
    if (face == Facing::UP) {
        return iconTop;
    }
    return icon;
}

// 4J-PB - Adding a TestUse for tooltip display
bool JukeboxTile::TestUse(Level* level, int x, int y, int z,
                          std::shared_ptr<Player> player) {
    // if the jukebox is empty, return true
    if (level->getData(x, y, z) == 0) return false;
    return true;
}

bool JukeboxTile::use(Level* level, int x, int y, int z,
                      std::shared_ptr<Player> player, int clickedFace,
                      float clickX, float clickY, float clickZ,
                      bool soundOnly /*=false*/)  // 4J added soundOnly param
{
    if (soundOnly) return false;
    if (level->getData(x, y, z) == 0) return false;
    dropRecording(level, x, y, z);
    return true;
}

void JukeboxTile::setRecord(Level* level, int x, int y, int z,
                            std::shared_ptr<ItemInstance> record) {
    if (level->isClientSide) return;

    std::shared_ptr<JukeboxTile::Entity> rte =
        std::dynamic_pointer_cast<JukeboxTile::Entity>(
            level->getTileEntity(x, y, z));
    rte->setRecord(record->copy());
    rte->setChanged();

    level->setData(x, y, z, 1, Tile::UPDATE_CLIENTS);
}

void JukeboxTile::dropRecording(Level* level, int x, int y, int z) {
    if (level->isClientSide) return;

    std::shared_ptr<JukeboxTile::Entity> rte =
        std::dynamic_pointer_cast<JukeboxTile::Entity>(
            level->getTileEntity(x, y, z));
    if (rte == nullptr) return;

    std::shared_ptr<ItemInstance> oldRecord = rte->getRecord();
    if (oldRecord == nullptr) return;

    level->levelEvent(LevelEvent::SOUND_PLAY_RECORDING, x, y, z, 0);
    // 4J-PB- the level event will play the music
    // level->playStreamingMusic("", x, y, z);
    rte->setRecord(nullptr);
    rte->setChanged();
    level->setData(x, y, z, 0, Tile::UPDATE_CLIENTS);

    float s = 0.7f;
    double xo = level->random->nextFloat() * s + (1 - s) * 0.5;
    double yo = level->random->nextFloat() * s + (1 - s) * 0.2 + 0.6;
    double zo = level->random->nextFloat() * s + (1 - s) * 0.5;

    std::shared_ptr<ItemInstance> itemInstance = oldRecord->copy();

    std::shared_ptr<ItemEntity> item = std::shared_ptr<ItemEntity>(
        new ItemEntity(level, x + xo, y + yo, z + zo, itemInstance));
    item->throwTime = 10;
    level->addEntity(item);
}

void JukeboxTile::onRemove(Level* level, int x, int y, int z, int id,
                           int data) {
    dropRecording(level, x, y, z);
    Tile::onRemove(level, x, y, z, id, data);
}

void JukeboxTile::spawnResources(Level* level, int x, int y, int z, int data,
                                 float odds, int playerBonus) {
    if (level->isClientSide) return;
    Tile::spawnResources(level, x, y, z, data, odds, 0);
}

std::shared_ptr<TileEntity> JukeboxTile::newTileEntity(Level* level) {
    return std::make_shared<JukeboxTile::Entity>();
}

void JukeboxTile::registerIcons(IconRegister* iconRegister) {
    icon = iconRegister->registerIcon(getIconName() + "_side");
    iconTop = iconRegister->registerIcon(getIconName() + "_top");
}

bool JukeboxTile::hasAnalogOutputSignal() { return true; }

int JukeboxTile::getAnalogOutputSignal(Level* level, int x, int y, int z,
                                       int dir) {
    std::shared_ptr<ItemInstance> record =
        std::dynamic_pointer_cast<JukeboxTile::Entity>(
            level->getTileEntity(x, y, z))
            ->getRecord();
    return record == nullptr ? Redstone::SIGNAL_NONE
                             : record->id + 1 - Item::record_01_Id;
}