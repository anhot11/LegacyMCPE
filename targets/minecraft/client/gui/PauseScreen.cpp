#include "PauseScreen.h"

#include <math.h>

#include <memory>
#include <numbers>
#include <string>
#include <vector>

#include "Button.h"
#include "MessageScreen.h"
#include "OptionsScreen.h"
#include "minecraft/GameEnums.h"
#include "minecraft/IGameServices.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/gui/Screen.h"
#include "minecraft/client/multiplayer/MultiPlayerLocalPlayer.h"
#include "minecraft/locale/I18n.h"
#include "minecraft/network/INetworkService.h"
#include "minecraft/server/MinecraftServer.h"
#include "minecraft/server/ServerAction.h"
#include "minecraft/world/item/Item.h"
#include "minecraft/world/item/ItemInstance.h"
#include "platform/input/input.h"

PauseScreen::PauseScreen() {
    saveStep = 0;
    visibleTime = 0;
}

void PauseScreen::init() {
    saveStep = 0;
    buttons.clear();
    int yo = -16;

    if (NetworkService.IsLocalGame() && NetworkService.GetPlayerCount() == 1)
        MinecraftServer::getInstance()->queueServerAction(
            minecraft::server::PauseServer{true});

    Button* btnReturnGame = new Button(4, width / 2 - 100, height / 4 + 24 * 1 + yo,
                                       I18n::get("menu.returnToGame"));
    btnReturnGame->setTextureIcon(0, 32, 16);
    btnReturnGame->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::sword_diamond)));
    buttons.push_back(btnReturnGame);

    Button* btnOptions = new Button(0, width / 2 - 100, height / 4 + 24 * 2 + yo + 4,
                                    I18n::get("menu.options"));
    btnOptions->setTextureIcon(48, 16, 16);
    btnOptions->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::repeater)));
    buttons.push_back(btnOptions);

    std::string quitMsg = NetworkService.IsHost() ? I18n::get("menu.returnToMenu") : I18n::get("menu.disconnect");
    Button* btnQuit = new Button(1, width / 2 - 100, height / 4 + 24 * 3 + yo + 8, quitMsg);
    btnQuit->setTextureIcon(96, 32, 16);
    btnQuit->setIconItem(std::shared_ptr<ItemInstance>(new ItemInstance(Item::door_iron)));
    buttons.push_back(btnQuit);
}

void PauseScreen::exitWorld(Minecraft* minecraft, bool save) {
    // 4jcraft: made our own static method for use in the java gui (other
    // places such as the deathscreen need this)
    MinecraftServer* server = MinecraftServer::getInstance();

    minecraft->setScreen(new MessageScreen("Leaving world"));
    if (NetworkService.IsHost()) {
        server->setSaveOnExit(save);
    }
    gameServices().setAction(minecraft->player->GetXboxPad(),
                             eAppAction_ExitWorld);
}

void PauseScreen::buttonClicked(Button* button) {
    if (button->id == 0) {
        minecraft->setScreen(new OptionsScreen(this, minecraft->options));
    }
    if (button->id == 1) {
        // if (minecraft->isClientSide())
        // {
        //     minecraft->level->disconnect();
        // }

        // minecraft->setLevel(nullptr);
        // minecraft->setScreen(new TitleScreen());

        // 4jcraft: exit with our new exitWorld method
        exitWorld(minecraft, true);
    }
    if (button->id == 4) {
        MinecraftServer::getInstance()->queueServerAction(
            minecraft::server::PauseServer{false});
        minecraft->setScreen(nullptr);
    }
}

void PauseScreen::tick() {
    Screen::tick();
    visibleTime++;
}

void PauseScreen::render(int xm, int ym, float a) {
    renderBackground();

    bool isSaving = false;  //! minecraft->level->pauseSave(saveStep++);
    if (isSaving || visibleTime < 20) {
        float col = ((visibleTime % 10) + a) / 10.0f;
        col = sinf(col * std::numbers::pi * 2) * 0.2f + 0.8f;
        int br = (int)(255 * col);

        drawString(font, "Saving level..", 8, height - 16,
                   br << 16 | br << 8 | br);
    }

    drawCenteredString(font, "Game menu", width / 2, 40, 0xffffff);

    Screen::render(xm, ym, a);
}