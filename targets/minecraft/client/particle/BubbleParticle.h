#pragma once
#include "Particle.h"
#include "java/Class.h"

class Level;

class BubbleParticle : public Particle {
public:
    virtual eINSTANCEOF GetType() { return eType_BUBBLEPARTICLE; }
    BubbleParticle(Level* level, double x, double y, double z, double xa,
                   double ya, double za);
    virtual void tick();
};