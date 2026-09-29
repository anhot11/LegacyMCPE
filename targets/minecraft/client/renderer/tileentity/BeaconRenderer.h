#pragma once
#include "TileEntityRenderer.h"

class BeaconTileEntity;
class ResourceLocation;

class BeaconRenderer : public TileEntityRenderer {
private:
    static ResourceLocation BEAM_LOCATION;

public:
    virtual void render(std::shared_ptr<TileEntity> _beacon, double x, double y,
                        double z, float a, bool setColor, float alpha,
                        bool useCompiled);
};
