#include "Rarity.h"

const Rarity* Rarity::common = new Rarity(eHTMLColor_f, "Common");
const Rarity* Rarity::uncommon = new Rarity(eHTMLColor_e, "Uncommon");
const Rarity* Rarity::rare = new Rarity(eHTMLColor_b, "Rare");
const Rarity* Rarity::epic = new Rarity(eHTMLColor_d, "Epic");

Rarity::Rarity(eMinecraftColour color, const std::string& name)
    : color(color), name(name) {}