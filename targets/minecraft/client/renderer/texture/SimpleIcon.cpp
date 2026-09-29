#include "SimpleIcon.h"

#include "minecraft/client/renderer/texture/StitchedTexture.h"

SimpleIcon::SimpleIcon(const std::string& name, const std::string& filename,
                       float U0, float V0, float U1, float V1)
    : StitchedTexture(name, filename) {
    u0 = U0;
    u1 = U1;
    v0 = V0;
    v1 = V1;
}
