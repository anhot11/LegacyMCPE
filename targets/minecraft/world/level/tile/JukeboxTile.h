#pragma once

#include <memory>

#include "BaseEntityTile.h"
#include "java/Class.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/tile/entity/TileEntity.h"
#include "nbt/CompoundTag.h"

class CompoundTag;
class ChunkRebuildData;
class Icon;
class Level;

class JukeboxTile : public BaseEntityTile {
    friend class Tile;
    friend class ChunkRebuildData;

public:
    class Entity : public TileEntity {
    public:
        eINSTANCEOF GetType() { return eTYPE_RECORDPLAYERTILE; }
        static TileEntity* create() { return new JukeboxTile::Entity(); }

    private:
        std::shared_ptr<ItemInstance> record;

    public:
        Entity();

        virtual void load(CompoundTag* tag);
        virtual void save(CompoundTag* tag);
        virtual std::shared_ptr<ItemInstance> getRecord();
        virtual void setRecord(std::shared_ptr<ItemInstance> record);

        // 4J Added
        std::shared_ptr<TileEntity> clone();
    };

private:
    Icon* iconTop;

protected:
    JukeboxTile(int id);

public:
    virtual Icon* getTexture(int face, int data);
    virtual bool TestUse(Level* level, int x, int y, int z,
                         std::shared_ptr<Player> player);
    virtual bool use(Level* level, int x, int y, int z,
                     std::shared_ptr<Player> player, int clickedFace,
                     float clickX, float clickY, float clickZ,
                     bool soundOnly = false);  // 4J added soundOnly param
    void setRecord(Level* level, int x, int y, int z,
                   std::shared_ptr<ItemInstance> record);
    void dropRecording(Level* level, int x, int y, int z);
    virtual void onRemove(Level* level, int x, int y, int z, int id, int data);
    virtual void spawnResources(Level* level, int x, int y, int z, int data,
                                float odds, int playerBonus);

    virtual std::shared_ptr<TileEntity> newTileEntity(Level* level);
    virtual void registerIcons(IconRegister* iconRegister);
    virtual bool hasAnalogOutputSignal();
    virtual int getAnalogOutputSignal(Level* level, int x, int y, int z,
                                      int dir);
};
