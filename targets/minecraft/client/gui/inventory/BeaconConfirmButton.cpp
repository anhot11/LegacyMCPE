#include "BeaconConfirmButton.h"

#include <string>

#include "BeaconScreen.h"
#include "minecraft/client/gui/inventory/AbstractBeaconButton.h"
#include "minecraft/locale/Language.h"

// 4jcraft: referenced from MCP 8.11 (JE 1.6.4)
#ifdef ENABLE_JAVA_GUIS
extern ResourceLocation GUI_BEACON_LOCATION;
#endif

BeaconConfirmButton::BeaconConfirmButton(BeaconScreen* screen, int id, int x,
                                         int y)
    : AbstractBeaconButton(id, x, y) {
    this->screen = screen;
#ifdef ENABLE_JAVA_GUIS
    this->iconRes = &GUI_BEACON_LOCATION;
#endif
    this->iconU = 90;
    this->iconV = 220;
}

void BeaconConfirmButton::renderTooltip(int xm, int ym) {
    screen->renderTooltip(Language::getInstance()->getElement("gui.done"), xm,
                          ym);
}