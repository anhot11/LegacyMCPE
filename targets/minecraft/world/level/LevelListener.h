#pragma once

class TileEntity;
#include "minecraft/core/particles/ParticleTypes.h"
#include "minecraft/world/entity/player/Player.h"

class LevelListener {
public:
    virtual void tileChanged(int x, int y, int z) = 0;

    virtual void tileLightChanged(int x, int y, int z) = 0;

    virtual void setTilesDirty(int x0, int y0, int z0, int x1, int y1, int z1,
                               Level* level) = 0;  // 4J - added level param

    virtual void allChanged() = 0;

    // virtual void playSound(const string& name, double x, double y, double z,
    // float volume, float pitch) = 0;
    virtual void playSound(int iSound, double x, double y, double z,
                           float volume, float pitch,
                           float fSoundClipDist = 16.0f) = 0;
    virtual void playSoundExceptPlayer(std::shared_ptr<Player> player,
                                       int iSound, double x, double y, double z,
                                       float volume, float pitch,
                                       float fSoundClipDist = 16.0f) = 0;

    // 4J removed - virtual void addParticle(const string& name, double x,
    // double y, double z, double xa, double ya, double za) = 0;

    virtual void addParticle(ePARTICLE_TYPE name, double x, double y, double z,
                             double xa, double ya, double za) = 0;  // 4J added

    virtual void entityAdded(std::shared_ptr<Entity> entity) = 0;

    virtual void entityRemoved(std::shared_ptr<Entity> entity) = 0;

    virtual void playerRemoved(
        std::shared_ptr<Entity>
            entity) = 0;  // 4J added - for when a player is removed from the
                          // level's player array, not just the entity storage

    virtual void skyColorChanged() = 0;

    virtual void playStreamingMusic(const std::string& name, int x, int y,
                                    int z) = 0;
    virtual void globalLevelEvent(int type, int sourceX, int sourceY,
                                  int sourceZ, int data) = 0;
    virtual void levelEvent(std::shared_ptr<Player> source, int type, int x,
                            int y, int z, int data) = 0;

    virtual void destroyTileProgress(int id, int x, int y, int z,
                                     int progress) = 0;
};