#pragma once
#include <memory>
#include <string>

#include "Tile.h"

class Icon;
class Material;

class ThinFenceTile : public Tile {
private:
    std::string edgeTexture;
    bool dropsResources;
    std::string texture;
    Icon* iconSide;

public:
    ThinFenceTile(int id, const std::string& tex, const std::string& edgeTex,
                  Material* material, bool dropsResources);
    virtual int getResource(int data, Random* random, int playerBonusLevel);
    virtual bool isSolidRender(bool isServerLevel = false);
    virtual bool isCubeShaped();
    virtual int getRenderShape();
    virtual bool shouldRenderFace(LevelSource* level, int x, int y, int z,
                                  int face);
    virtual void addAABBs(Level* level, int x, int y, int z, AABB* box,
                          std::vector<AABB>* boxes,
                          std::shared_ptr<Entity> source);
    virtual void updateDefaultShape();
    virtual void updateShape(
        LevelSource* level, int x, int y, int z, int forceData = -1,
        std::shared_ptr<TileEntity> forceEntity = std::shared_ptr<
            TileEntity>());  // 4J added forceData, forceEntity param
    virtual Icon* getEdgeTexture();
    bool attachsTo(int tile);

protected:
    bool isSilkTouchable();
    std::shared_ptr<ItemInstance> getSilkTouchItemInstance(int data);

public:
    virtual void registerIcons(IconRegister* iconRegister);
};
