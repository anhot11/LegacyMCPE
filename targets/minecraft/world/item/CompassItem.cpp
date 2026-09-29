#include "CompassItem.h"

#include <memory>

#include "minecraft/client/Minecraft.h"
#include "minecraft/client/multiplayer/MultiPlayerLocalPlayer.h"
#include "minecraft/world/IconRegister.h"
#include "minecraft/world/item/Item.h"

class Icon;

const std::string CompassItem::TEXTURE_PLAYER_ICON[XUSER_MAX_COUNT] = {
    "compassP0", "compassP1", "compassP2", "compassP3"};

CompassItem::CompassItem(int id) : Item(id) { icons = nullptr; }

// 4J Added so that we can override the icon id used to calculate the texture
// UV's for each player

Icon* CompassItem::getIcon(int auxValue) {
    Icon* icon = Item::getIcon(auxValue);
    Minecraft* pMinecraft = Minecraft::GetInstance();

    if (pMinecraft->player != nullptr && auxValue == 0) {
        icon = icons[pMinecraft->player->GetXboxPad()];
    }
    return icon;
}

void CompassItem::registerIcons(IconRegister* iconRegister) {
    Item::registerIcons(iconRegister);

    icons = new Icon*[XUSER_MAX_COUNT];

    for (int i = 0; i < XUSER_MAX_COUNT; i++) {
        icons[i] = iconRegister->registerIcon(TEXTURE_PLAYER_ICON[i]);
    }
}
