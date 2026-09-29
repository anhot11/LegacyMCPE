#pragma once
#include <memory>
#include <string>

#include "Tile.h"

class LevelSource;
class Material;

class FenceTile : public Tile {
private:
    std::string texture;

public:
    FenceTile(int id, const std::string& texture, Material* material);
    virtual void addAABBs(Level* level, int x, int y, int z, AABB* box,
                          std::vector<AABB>* boxes,
                          std::shared_ptr<Entity> source);
    virtual void updateShape(
        LevelSource* level, int x, int y, int z, int forceData = -1,
        std::shared_ptr<TileEntity> forceEntity = std::shared_ptr<
            TileEntity>());  // 4J added forceData, forceEntity param
    virtual bool isSolidRender(bool isServerLevel = false);
    virtual bool isCubeShaped();
    virtual bool isPathfindable(LevelSource* level, int x, int y, int z);
    virtual bool shouldRenderFace(LevelSource* level, int x, int y, int z,
                                  int face);
    virtual int getRenderShape();
    virtual bool connectsTo(LevelSource* level, int x, int y, int z);
    static bool isFence(int tile);
    virtual void registerIcons(IconRegister* iconRegister);
    virtual bool use(Level* level, int x, int y, int z,
                     std::shared_ptr<Player> player, int clickedFace,
                     float clickX, float clickY, float clickZ,
                     bool soundOnly = false);
};