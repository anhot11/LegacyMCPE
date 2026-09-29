#pragma once
#include "WaterDropParticle.h"
#include "java/Class.h"

class Level;

class SplashParticle : public WaterDropParticle {
public:
    virtual eINSTANCEOF GetType() { return eType_SPLASHPARTICLE; }
    SplashParticle(Level* level, double x, double y, double z, double xa,
                   double ya, double za);
};