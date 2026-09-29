#pragma once
#include "Particle.h"
#include "java/Class.h"

class Textures;
class Level;
class ResourceLocation;

class FootstepParticle : public Particle {
public:
    virtual eINSTANCEOF GetType() { return eType_FOOTSTEPPARTICLE; }

private:
    static ResourceLocation FOOTPRINT_LOCATION;
    int life;
    int lifeTime;
    Textures* textures;

public:
    FootstepParticle(Textures* textures, Level* level, double x, double y,
                     double z);
    virtual void render(Tesselator* t, float a, float xa, float ya, float za,
                        float xa2, float za2);
    virtual void tick();
    virtual int getParticleTexture();
};