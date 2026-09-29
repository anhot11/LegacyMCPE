#include "Emboss.h"

#include "minecraft/world/level/levelgen/synth/Synth.h"

Emboss::Emboss(Synth* synth) { this->synth = synth; }

double Emboss::getValue(double x, double y) {
    return synth->getValue(x, y) - synth->getValue(x + 1, y + 1);
}