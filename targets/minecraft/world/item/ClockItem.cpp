#include "ClockItem.h"

#include <memory>

#include "minecraft/client/Minecraft.h"
#include "minecraft/client/multiplayer/MultiPlayerLocalPlayer.h"
#include "minecraft/world/IconRegister.h"
#include "minecraft/world/item/Item.h"

class Icon;

const std::string ClockItem::TEXTURE_PLAYER_ICON[XUSER_MAX_COUNT] = {
    "clockP0", "clockP1", "clockP2", "clockP3"};

ClockItem::ClockItem(int id) : Item(id) { icons = nullptr; }

// 4J Added so that we can override the icon id used to calculate the texture
// UV's for each player
Icon* ClockItem::getIcon(int auxValue) {
    Icon* icon = Item::getIcon(auxValue);
    Minecraft* pMinecraft = Minecraft::GetInstance();

    if (pMinecraft->player != nullptr && auxValue == 0) {
        icon = icons[pMinecraft->player->GetXboxPad()];
    }
    return icon;
}

void ClockItem::registerIcons(IconRegister* iconRegister) {
    Item::registerIcons(iconRegister);

    icons = new Icon*[XUSER_MAX_COUNT];

    for (int i = 0; i < XUSER_MAX_COUNT; i++) {
        icons[i] = iconRegister->registerIcon(TEXTURE_PLAYER_ICON[i]);
    }
}
