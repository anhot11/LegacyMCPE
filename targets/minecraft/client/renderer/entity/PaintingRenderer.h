#pragma once
#include <memory>

#include "EntityRenderer.h"

class Painting;
class Random;
class ResourceLocation;

class PaintingRenderer : public EntityRenderer {
private:
    Random* random;
    static ResourceLocation PAINTING_LOCATION;

public:
    PaintingRenderer();  // 4J -added
    virtual void render(std::shared_ptr<Entity> _painting, double x, double y,
                        double z, float rot, float a);

private:
    void renderPainting(std::shared_ptr<Painting> painting, int w, int h,
                        int uo, int vo);
    void setBrightness(std::shared_ptr<Painting> painting, float ss, float ya);
    virtual ResourceLocation* getTextureLocation(std::shared_ptr<Entity> mob);
};
