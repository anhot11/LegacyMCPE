#pragma once

#include <memory>

#include "Particle.h"
#include "java/Class.h"
#include "minecraft/core/particles/ParticleTypes.h"

class Entity;
class Level;

class CritParticle : public Particle {
private:
    std::shared_ptr<Entity> entity;
    int life;
    int lifeTime;
    ePARTICLE_TYPE particleName;

    void _init(Level* level, std::shared_ptr<Entity> entity,
               ePARTICLE_TYPE type);

public:
    virtual eINSTANCEOF GetType() { return eType_CRITPARTICLE; }
    CritParticle(Level* level, std::shared_ptr<Entity> entity);
    CritParticle(Level* level, std::shared_ptr<Entity> entity,
                 ePARTICLE_TYPE type);
    void CritParticlePostConstructor(void);
    void render(Tesselator* t, float a, float xa, float ya, float za, float xa2,
                float za2);
    void tick();
    int getParticleTexture();
};