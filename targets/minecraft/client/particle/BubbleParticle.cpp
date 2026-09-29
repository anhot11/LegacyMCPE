#include "BubbleParticle.h"

#include <cmath>

#include "java/JavaMath.h"
#include "java/Random.h"
#include "minecraft/client/particle/Particle.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/material/Material.h"

BubbleParticle::BubbleParticle(Level* level, double x, double y, double z,
                               double xa, double ya, double za)
    : Particle(level, x, y, z, xa, ya, za) {
    rCol = 1.0f;
    gCol = 1.0f;
    bCol = 1.0f;
    setMiscTex(32);
    this->setSize(0.02f, 0.02f);

    size = size * (random->nextFloat() * 0.6f + 0.2f);

    xd = xa * 0.2f + (float)(Math::random() * 2 - 1) * 0.02f;
    yd = ya * 0.2f + (float)(Math::random() * 2 - 1) * 0.02f;
    zd = za * 0.2f + (float)(Math::random() * 2 - 1) * 0.02f;

    lifetime = (int)(8 / (Math::random() * 0.8 + 0.2));
}

void BubbleParticle::tick() {
    xo = x;
    yo = y;
    zo = z;

    yd += 0.002;
    move(xd, yd, zd);
    xd *= 0.85f;
    yd *= 0.85f;
    zd *= 0.85f;

    if (level->getMaterial(std::floor(x), std::floor(y), std::floor(z)) !=
        Material::water)
        remove();

    if (lifetime-- <= 0) remove();
}