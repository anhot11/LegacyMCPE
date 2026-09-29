#pragma once
#include <format>
#include <memory>
#include <string>
#include <vector>

#include "minecraft/client/gui/Screen.h"

class ItemRenderer;
class AbstractContainerMenu;
class Slot;
class Container;
class ItemInstance;

class AbstractContainerScreen : public Screen {
protected:
    // 4jcraft: made protected to match JE 1.6.4
    static ItemRenderer* itemRenderer;

protected:
    int imageWidth;
    int imageHeight;
    // int leftPos, topPos;
public:
    AbstractContainerMenu* menu;

    AbstractContainerScreen(AbstractContainerMenu* menu);
    virtual void init() override;
    virtual void render(int xm, int ym, float a) override;

protected:
    virtual void renderLabels();
    virtual void renderBg(float a) = 0;
    // 4jcraft: promoted from private to protected so CreativeInventoryScreen
    // can call findSlot() and isHovering() directly for its custom click
    // handling.
    virtual Slot* findSlot(int x, int y);
    // 4jcraft: equivalent to MCP 8.11 (1.6.x)'s GuiContainer.isPointInRegion()
    // for use in other derived classes
    virtual bool isHoveringOver(int x, int y, int w, int h, int xm, int ym);
    virtual bool isHovering(Slot* slot, int xm, int ym);
    // 4jcraft: extracted from render() into a standalone method so this can be
    // used in other places
    virtual void renderTooltipInternal(
        const std::vector<std::string>& cleanedLines,
        const std::vector<int>& lineColors, int xm, int ym);
    virtual void renderTooltip(std::shared_ptr<ItemInstance> item, int xm,
                               int ym);

private:
    virtual void renderSlot(Slot* slot);

protected:
    virtual void mouseClicked(int x, int y, int buttonNum) override;
    virtual void mouseReleased(int x, int y, int buttonNum) override;
    virtual void keyPressed(char eventCharacter, int eventKey) override;

public:
    virtual void removed() override;
    virtual void slotsChanged(std::shared_ptr<Container> container);
    virtual bool isPauseScreen() override;
    virtual void tick() override;

    // 4jcraft: 1.6.x era overloads
    virtual void renderTooltip(const std::vector<std::string>& lines, int xm,
                               int ym);
    virtual void renderTooltip(const std::string& line, int xm, int ym);
};
