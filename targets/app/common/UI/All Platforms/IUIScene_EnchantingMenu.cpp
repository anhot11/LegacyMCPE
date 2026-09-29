#include "IUIScene_EnchantingMenu.h"

#include <assert.h>

#include <memory>

#include "app/common/UI/All Platforms/IUIScene_AbstractContainerMenu.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/multiplayer/MultiPlayerGameMode.h"
#include "minecraft/client/multiplayer/MultiPlayerLocalPlayer.h"
#include "minecraft/world/inventory/AbstractContainerMenu.h"
#include "minecraft/world/inventory/EnchantmentMenu.h"

class Player;

IUIScene_AbstractContainerMenu::ESceneSection
IUIScene_EnchantingMenu::GetSectionAndSlotInDirection(
    IUIScene_AbstractContainerMenu::ESceneSection eSection,
    ETapState eTapDirection, int* piTargetX, int* piTargetY) {
    IUIScene_AbstractContainerMenu::ESceneSection newSection = eSection;
    int xOffset = 0;

    // Find the new section if there is one
    switch (eSection) {
        case eSectionEnchantInventory:
            if (eTapDirection == eTapStateDown) {
                newSection = eSectionEnchantUsing;
            } else if (eTapDirection == eTapStateUp) {
                if (*piTargetX >= ENCHANT_SCENE_ENCHANT_BUTTONS_UP_OFFSET) {
                    newSection = eSectionEnchantButton3;
                } else {
                    newSection = eSectionEnchantSlot;
                }
            }
            break;
        case eSectionEnchantUsing:
            if (eTapDirection == eTapStateDown) {
                if (*piTargetX >= ENCHANT_SCENE_ENCHANT_BUTTONS_UP_OFFSET) {
                    newSection = eSectionEnchantButton1;
                } else {
                    newSection = eSectionEnchantSlot;
                }
            } else if (eTapDirection == eTapStateUp) {
                newSection = eSectionEnchantInventory;
            }
            break;
        case eSectionEnchantSlot:
            if (eTapDirection == eTapStateDown) {
                newSection = eSectionEnchantInventory;
                xOffset = ENCHANT_SCENE_INGREDIENT_SLOT_DOWN_OFFSET;
            } else if (eTapDirection == eTapStateUp) {
                newSection = eSectionEnchantUsing;
                xOffset = ENCHANT_SCENE_INGREDIENT_SLOT_DOWN_OFFSET;
            } else if (eTapDirection == eTapStateLeft ||
                       eTapDirection == eTapStateRight) {
                newSection = eSectionEnchantButton1;
            }
            break;
        case eSectionEnchantButton1:
            if (eTapDirection == eTapStateDown) {
                newSection = eSectionEnchantButton2;
            } else if (eTapDirection == eTapStateUp) {
                newSection = eSectionEnchantUsing;
                xOffset = ENCHANT_SCENE_ENCHANT_BUTTONS_DOWN_OFFSET;
            } else if (eTapDirection == eTapStateLeft ||
                       eTapDirection == eTapStateRight) {
                newSection = eSectionEnchantSlot;
            }
            break;
        case eSectionEnchantButton2:
            if (eTapDirection == eTapStateDown) {
                newSection = eSectionEnchantButton3;
            } else if (eTapDirection == eTapStateUp) {
                newSection = eSectionEnchantButton1;
            } else if (eTapDirection == eTapStateLeft ||
                       eTapDirection == eTapStateRight) {
                newSection = eSectionEnchantSlot;
            }
            break;
        case eSectionEnchantButton3:
            if (eTapDirection == eTapStateDown) {
                newSection = eSectionEnchantInventory;
                xOffset = ENCHANT_SCENE_ENCHANT_BUTTONS_DOWN_OFFSET;
            } else if (eTapDirection == eTapStateUp) {
                newSection = eSectionEnchantButton2;
            } else if (eTapDirection == eTapStateLeft ||
                       eTapDirection == eTapStateRight) {
                newSection = eSectionEnchantSlot;
            }
            break;
        default:
            assert(false);
            break;
    }

    updateSlotPosition(eSection, newSection, eTapDirection, piTargetX,
                       piTargetY, xOffset);

    return newSection;
}

void IUIScene_EnchantingMenu::handleOtherClicked(int iPad,
                                                 ESceneSection eSection,
                                                 int buttonNum, bool quickKey) {
    int index = -1;
    // Old xui code

    switch (eSection) {
        case eSectionEnchantButton1:
            index = 0;
            break;
        case eSectionEnchantButton2:
            index = 1;
            break;
        case eSectionEnchantButton3:
            index = 2;
            break;
        default:
            break;
    };
    Minecraft* pMinecraft = Minecraft::GetInstance();
    if (index >= 0 &&
        m_menu->clickMenuButton(
            std::dynamic_pointer_cast<Player>(pMinecraft->localplayers[iPad]),
            index)) {
        pMinecraft->localgameModes[iPad]->handleInventoryButtonClick(
            m_menu->containerId, index);
    }
}

int IUIScene_EnchantingMenu::getSectionStartOffset(ESceneSection eSection) {
    int offset = 0;
    switch (eSection) {
        case eSectionEnchantSlot:
            offset = 0;
            break;
        case eSectionEnchantInventory:
            offset = 1;
            break;
        case eSectionEnchantUsing:
            offset = 1 + 27;
            break;
        default:
            assert(false);
            break;
    };
    return offset;
}

bool IUIScene_EnchantingMenu::IsSectionSlotList(ESceneSection eSection) {
    switch (eSection) {
        case eSectionEnchantInventory:
        case eSectionEnchantUsing:
        case eSectionEnchantSlot:
            return true;
        default:
            break;
    }
    return false;
}

EnchantmentMenu* IUIScene_EnchantingMenu::getMenu() {
    return (EnchantmentMenu*)m_menu;
}
