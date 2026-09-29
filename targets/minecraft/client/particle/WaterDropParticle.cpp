#include "WaterDropParticle.h"

#include <cmath>

#include "java/JavaMath.h"
#include "java/Random.h"
#include "minecraft/client/particle/Particle.h"
#include "minecraft/world/level/Level.h"
#include "minecraft/world/level/material/Material.h"
#include "minecraft/world/level/tile/LiquidTile.h"

WaterDropParticle::WaterDropParticle(Level* level, double x, double y, double z)
    : Particle(level, x, y, z, 0, 0, 0) {
    xd *= 0.3f;
    yd = (float)Math::random() * 0.2f + 0.1f;
    zd *= 0.3f;

    rCol = 1.0f;
    gCol = 1.0f;
    bCol = 1.0f;
    setMiscTex(16 + 3 + random->nextInt(4));
    this->setSize(0.01f, 0.01f);
    gravity = 0.06f;

    noPhysics = true;  // 4J - optimisation - do we really need collision on
                       // these? its really slow...
    lifetime = (int)(8 / (Math::random() * 0.8 + 0.2));
}

void WaterDropParticle::tick() {
    xo = x;
    yo = y;
    zo = z;

    yd -= gravity;
    move(xd, yd, zd);
    xd *= 0.98f;
    yd *= 0.98f;
    zd *= 0.98f;

    if (lifetime-- <= 0) remove();

    if (onGround) {
        if (Math::random() < 0.5) remove();
        xd *= 0.7f;
        zd *= 0.7f;
    }

    Material* m =
        level->getMaterial(std::floor(x), std::floor(y), std::floor(z));
    if (m->isLiquid() || m->isSolid()) {
        double y0 = std::floor(y) + 1 -
                    LiquidTile::getHeight(level->getData(
                        std::floor(x), std::floor(y), std::floor(z)));
        if (y < y0) {
            remove();
        }
    }
}
