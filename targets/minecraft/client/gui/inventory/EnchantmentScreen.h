#pragma once

#include <memory>
#include <string>
#include <vector>

#include "AbstractContainerScreen.h"
#include "java/Random.h"
#include "minecraft/world/inventory/EnchantmentMenu.h"

class EnchantmentMenu;
class Inventory;
class ItemInstance;
class Level;

class EnchantmentScreen : public AbstractContainerScreen {
public:
    EnchantmentScreen(std::shared_ptr<Inventory> inventory, Level* level, int x,
                      int y, int z);
    virtual ~EnchantmentScreen();

    void init() override;
    void removed() override;
    void tick() override;
    void mouseClicked(int mouseX, int mouseY, int buttonNum) override;
    void renderLabels() override;
    void renderBg(float a) override;
    void render(int xm, int ym, float a) override;

private:
    std::shared_ptr<Inventory> inventory;
    EnchantmentMenu* enchantMenu;
    float xMouse, yMouse;

    Random random;

    // 4jcraft: brought over from UIControl_EnchantmentBook
    int bookTick;
    float flip, oFlip, flipT, flipA;
    float open, oOpen;
    std::shared_ptr<ItemInstance> last;

    // 4jcraft: brought over from UIControl_EnchantmentButton
    class EnchantmentNames {
    public:
        static EnchantmentNames instance;

    private:
        Random random;
        std::vector<std::string> words;

        EnchantmentNames();

    public:
        std::string getRandomName();
    };

    std::string enchantNames[3];
    int lastCosts[3];
};