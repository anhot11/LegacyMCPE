#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "TileEntityRenderer.h"

class BaseMobSpawner;
class Entity;

class MobSpawnerRenderer : public TileEntityRenderer {
private:
    std::unordered_map<std::string, std::shared_ptr<Entity> > models;

public:
    static void render(BaseMobSpawner* spawner, double x, double y, double z,
                       float a);
    virtual void render(std::shared_ptr<TileEntity> _spawner, double x,
                        double y, double z, float a, bool setColor,
                        float alpha = 1.0f,
                        bool useCompiled = true);  // 4J added setColor param
};
