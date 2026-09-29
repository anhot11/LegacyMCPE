#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include "minecraft/world/level/tile/Tile.h"

class Icon;
class Level;
class LivingEntity;

class PistonBaseTile : public Tile {
public:
    static const int EXTENDED_BIT = 8;
    static const int UNDEFINED_FACING = 7;

    static const float PLATFORM_THICKNESS;
    static const int MAX_PUSH_DEPTH = 12;
    static const int TRIGGER_EXTEND = 0;
    static const int TRIGGER_CONTRACT = 1;

    static const std::string EDGE_TEX;
    static const std::string PLATFORM_TEX;
    static const std::string PLATFORM_STICKY_TEX;
    static const std::string BACK_TEX;
    static const std::string INSIDE_TEX;

private:
    bool isSticky;

    Icon* iconInside;
    Icon* iconBack;
    Icon* iconPlatform;

    static thread_local bool m_tlsIgnoreUpdate;

    // 4J - was just a static but implemented with TLS for our version
    static bool ignoreUpdate();
    static void ignoreUpdate(bool set);

public:
    PistonBaseTile(int id, bool isSticky);

    Icon* getPlatformTexture();
    virtual void updateShape(float x0, float y0, float z0, float x1, float y1,
                             float z1);

    virtual Icon* getTexture(int face, int data);
    static Icon* getTexture(const std::string& name);
    void registerIcons(IconRegister* iconRegister);

    virtual int getRenderShape();
    virtual bool isSolidRender(bool isServerLevel = false);
    virtual bool use(Level* level, int x, int y, int z,
                     std::shared_ptr<Player> player, int clickedFace,
                     float clickX, float clickY, float clickZ,
                     bool soundOnly = false);  // 4J added soundOnly param
    virtual void setPlacedBy(Level* level, int x, int y, int z,
                             std::shared_ptr<LivingEntity> by,
                             std::shared_ptr<ItemInstance> itemInstance);
    virtual void neighborChanged(Level* level, int x, int y, int z, int type);
    virtual void onPlace(Level* level, int x, int y, int z);

private:
    void checkIfExtend(Level* level, int x, int y, int z);
    bool getNeighborSignal(Level* level, int x, int y, int z, int facing);

public:
    virtual bool triggerEvent(Level* level, int x, int y, int z, int param1,
                              int facing);
    virtual void updateShape(
        LevelSource* level, int x, int y, int z, int forceData = -1,
        std::shared_ptr<TileEntity> forceEntity = std::shared_ptr<
            TileEntity>());  // 4J added forceData, forceEntity param
    virtual void updateDefaultShape();
    virtual void addAABBs(Level* level, int x, int y, int z, AABB* box,
                          std::vector<AABB>* boxes,
                          std::shared_ptr<Entity> source);
    virtual std::optional<AABB> getAABB(Level* level, int x, int y, int z);
    virtual bool isCubeShaped();

    static int getFacing(int data);
    static bool isExtended(int data);
    static int getNewFacing(Level* level, int x, int y, int z,
                            std::shared_ptr<LivingEntity> player);

private:
    static bool isPushable(int block, Level* level, int cx, int cy, int cz,
                           bool allowDestroyable);
    static bool canPush(Level* level, int sx, int sy, int sz, int facing);
    static void stopSharingIfServer(Level* level, int x, int y,
                                    int z);  // 4J added

    bool createPush(Level* level, int sx, int sy, int sz, int facing);
};
