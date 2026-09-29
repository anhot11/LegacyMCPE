#include "ContainerMenu.h"

#include <vector>

#include "Slot.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/player/LocalPlayer.h"
#include "minecraft/stats/GenericStats.h"
#include "minecraft/stats/StatsCounter.h"
#include "minecraft/world/Container.h"
#include "minecraft/world/entity/player/Player.h"
#include "minecraft/world/inventory/AbstractContainerMenu.h"
#include "minecraft/world/item/ItemInstance.h"
#include "minecraft/world/level/tile/Tile.h"

ContainerMenu::ContainerMenu(std::shared_ptr<Container> inventory,
                             std::shared_ptr<Container> container)
    : AbstractContainerMenu() {
    this->container = container;
    containerRows = container->getContainerSize() / 9;
    container->startOpen();

    int yo = (containerRows - 4) * 18;

    for (int y = 0; y < containerRows; y++) {
        for (int x = 0; x < 9; x++) {
            addSlot(new Slot(container, x + y * 9, 8 + x * 18, 18 + y * 18));
        }
    }

    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 9; x++) {
            addSlot(new Slot(inventory, x + y * 9 + 9, 8 + x * 18,
                             103 + y * 18 + yo));
        }
    }
    for (int x = 0; x < 9; x++) {
        addSlot(new Slot(inventory, x, 8 + x * 18, 161 + yo));
    }
}

bool ContainerMenu::stillValid(std::shared_ptr<Player> player) {
    return container->stillValid(player);
}

std::shared_ptr<ItemInstance> ContainerMenu::quickMoveStack(
    std::shared_ptr<Player> player, int slotIndex) {
    std::shared_ptr<ItemInstance> clicked = nullptr;
    Slot* slot = slots.at(slotIndex);
    if (slot != nullptr && slot->hasItem()) {
        std::shared_ptr<ItemInstance> stack = slot->getItem();
        clicked = stack->copy();

        if (slotIndex < containerRows * 9) {
            if (!moveItemStackTo(stack, containerRows * 9, (int)slots.size(),
                                 true)) {
                // 4J Stu - Brought forward from 1.2
                return nullptr;
            }
        } else {
            if (!moveItemStackTo(stack, 0, containerRows * 9, false)) {
                // 4J Stu - Brought forward from 1.2
                return nullptr;
            }
        }
        if (stack->count == 0) {
            slot->set(nullptr);
        } else {
            slot->setChanged();
        }
    }
    return clicked;
}

void ContainerMenu::removed(std::shared_ptr<Player> player) {
    AbstractContainerMenu::removed(player);
    container->stopOpen();
}

std::shared_ptr<Container> ContainerMenu::getContainer() { return container; }

std::shared_ptr<ItemInstance> ContainerMenu::clicked(
    int slotIndex, int buttonNum, int clickType, std::shared_ptr<Player> player,
    bool looped)  // 4J Added looped param
{
    std::shared_ptr<ItemInstance> out = AbstractContainerMenu::clicked(
        slotIndex, buttonNum, clickType, player, looped);

#if defined(_EXTENDED_ACHIEVEMENTS)
    std::shared_ptr<LocalPlayer> localPlayer =
        std::dynamic_pointer_cast<LocalPlayer>(player);

    if (localPlayer !=
        nullptr)  // 4J-JEV: For "Chestful o'Cobblestone" achievement.
    {
        int cobblecount = 0;
        for (int i = 0; i < container->getContainerSize(); i++) {
            std::shared_ptr<ItemInstance> item = container->getItem(i);
            if ((item != nullptr) && (item->id == Tile::cobblestone_Id)) {
                cobblecount += item->GetCount();
            }
        }

        // 4J-JEV: This check performed on XboxOne servers, for other platforms
        // check here.
        StatsCounter* sc =
            Minecraft::GetInstance()->stats[localPlayer->GetXboxPad()];
        int minedCount =
            sc->getTotalValue(GenericStats::blocksMined(Tile::stone_Id)) +
            sc->getTotalValue(GenericStats::blocksMined(Tile::cobblestone_Id));
        if (cobblecount >= 1728 && minedCount >= 1728) {
            localPlayer->awardStat(
                GenericStats::chestfulOfCobblestone(),
                GenericStats::param_chestfulOfCobblestone(cobblecount));
        }
    }
#endif

    return out;
}
