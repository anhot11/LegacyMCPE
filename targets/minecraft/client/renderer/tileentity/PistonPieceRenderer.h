#pragma once
#include "TileEntityRenderer.h"

class PistonPieceEntity;
class TileRenderer;
class ResourceLocation;

class PistonPieceRenderer : public TileEntityRenderer {
private:
    static ResourceLocation SIGN_LOCATION;
    TileRenderer* tileRenderer;

public:
    PistonPieceRenderer();
    virtual void render(std::shared_ptr<TileEntity> _entity, double x, double y,
                        double z, float a, bool setColor, float alpha = 1.0f,
                        bool useCompiled = true);  // 4J added setColor param
    virtual void onNewLevel(Level* level);
};
