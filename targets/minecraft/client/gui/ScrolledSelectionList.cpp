#include "ScrolledSelectionList.h"

#include <chrono>

#include "Button.h"
#include "GuiComponent.h"
#include "minecraft/client/Minecraft.h"
#include "minecraft/client/renderer/Tesselator.h"
#include "platform/input/input.h"
#include "platform/renderer/renderer.h"
#include "platform/stubs.h"

ScrolledSelectionList::ScrolledSelectionList(Minecraft* minecraft, int width,
                                             int height, int y0, int y1,
                                             int itemHeight) {
    this->minecraft = minecraft;
    this->width = width;
    this->height = height;
    this->y0 = y0;
    this->y1 = y1;
    this->itemHeight = itemHeight;
    this->x0 = 0;
    this->x1 = width;

    upId = 0;
    downId = 0;

    yDrag = -1.0f;
    yDragScale = 1.0f;
    yo = 0.0f;

    lastSelection = -1;
    lastSelectionTime = 0;

    renderSelection = true;
    _renderHeader = false;
    headerHeight = 0;
}

void ScrolledSelectionList::setRenderSelection(bool renderSelection) {
    this->renderSelection = renderSelection;
}

void ScrolledSelectionList::setRenderHeader(bool renderHeader,
                                            int headerHeight) {
    this->_renderHeader = renderHeader;
    this->headerHeight = headerHeight;

    if (!_renderHeader) {
        this->headerHeight = 0;
    }
}

int ScrolledSelectionList::getMaxPosition() {
    return getNumberOfItems() * itemHeight + headerHeight;
}

void ScrolledSelectionList::renderHeader(int x, int y, Tesselator* t) {}

void ScrolledSelectionList::clickedHeader(int headerMouseX, int headerMouseY) {}

void ScrolledSelectionList::renderDecorations(int mouseX, int mouseY) {}

int ScrolledSelectionList::getItemAtPosition(int x, int y) {
    int minX = width / 2 - 110;
    int maxX = width / 2 + 110;

    int clickSlotPos = (y - y0 - headerHeight + (int)yo - 4);
    int slot = clickSlotPos / itemHeight;
    if (x >= minX && x <= maxX && slot >= 0 && clickSlotPos >= 0 &&
        slot < getNumberOfItems()) {
        return slot;
    }
    return -1;
}

void ScrolledSelectionList::init(std::vector<Button*>* buttons, int upButtonId,
                                 int downButtonId) {
    this->upId = upButtonId;
    this->downId = downButtonId;
}

void ScrolledSelectionList::capYPosition() {
    int max = getMaxPosition() - (y1 - y0 - 4);
    if (max < 0) max = 0;
    if (yo < 0) yo = 0;
    if (yo > max) yo = (float)max;
}

void ScrolledSelectionList::buttonClicked(Button* button) {
    if (!button->active) return;

    if (button->id == upId) {
        yo -= (itemHeight * 2) / 3;
        yDrag = DRAG_OUTSIDE;
        capYPosition();
    } else if (button->id == downId) {
        yo += (itemHeight * 2) / 3;
        yDrag = DRAG_OUTSIDE;
        capYPosition();
    }
}

void ScrolledSelectionList::render(int xm, int ym, float a) {
    bool isDown = PlatformInput.ButtonDown(0, MINECRAFT_ACTION_ACTION);
    if (isDown) {
        if (yDrag == NO_DRAG) {
            if (ym >= y0 && ym <= y1) {
                int slot = getItemAtPosition(xm, ym);
                if (slot >= 0) {
                    auto now = std::chrono::steady_clock::now();
                    int64_t nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
                    bool doubleClick = (slot == lastSelection && (nowMs - lastSelectionTime < 350));
                    selectItem(slot, doubleClick);
                    lastSelection = slot;
                    lastSelectionTime = nowMs;
                }
                yDrag = (float)ym;
            } else {
                yDrag = DRAG_OUTSIDE;
            }
        } else if (yDrag >= 0.0f) {
            yo -= ((float)ym - yDrag) * yDragScale;
            yDrag = (float)ym;
        }
    } else {
        yDrag = NO_DRAG;
    }

    capYPosition();

    GuiComponent::fill(0, y0, width, y1, 0xb0000000);

    Tesselator* t = Tesselator::getInstance();
    int itemCount = getNumberOfItems();
    int minX = width / 2 - 110;

    int fbw = minecraft->width, fbh = minecraft->height;
    PlatformRenderer.GetFramebufferSize(fbw, fbh);

    glEnable(GL_SCISSOR_TEST);
    int scX = 0;
    int scW = fbw;
    int scY = (height - y1) * fbh / height;
    int scH = (y1 - y0) * fbh / height;
    glScissor(scX, scY < 0 ? 0 : scY, scW, scH < 0 ? 0 : scH);

    for (int i = 0; i < itemCount; i++) {
        int itemY = y0 + 4 - (int)yo + i * itemHeight;
        int h = itemHeight - 4;

        if (itemY <= y1 && itemY + itemHeight >= y0) {
            if (renderSelection && isSelectedItem(i)) {
                GuiComponent::fill(minX - 4, itemY - 2, minX + 224, itemY + h + 2, 0x80ffffff);
                GuiComponent::fill(minX - 3, itemY - 1, minX + 223, itemY + h + 1, 0xd0000000);
            }
            renderItem(i, minX, itemY, h, t);
        }
    }

    glDisable(GL_SCISSOR_TEST);

    GuiComponent::fillGradient(0, y0, width, y0 + 4, 0xff000000, 0x00000000);
    GuiComponent::fillGradient(0, y1 - 4, width, y1, 0x00000000, 0xff000000);

    int maxScroll = getMaxPosition() - (y1 - y0 - 4);
    if (maxScroll > 0) {
        int scrollBarX0 = width / 2 + 118;
        int scrollBarX1 = scrollBarX0 + 6;
        int barHeight = (y1 - y0) * (y1 - y0) / getMaxPosition();
        if (barHeight < 32) barHeight = 32;
        if (barHeight > y1 - y0 - 8) barHeight = y1 - y0 - 8;
        int barY = (int)yo * (y1 - y0 - barHeight) / maxScroll + y0;
        if (barY < y0) barY = y0;

        GuiComponent::fill(scrollBarX0, y0, scrollBarX1, y1, 0x60000000);
        GuiComponent::fill(scrollBarX0, barY, scrollBarX1, barY + barHeight, 0x80808080);
        GuiComponent::fill(scrollBarX0, barY, scrollBarX1 - 1, barY + barHeight - 1, 0xc0c0c0c0);
    }
}

void ScrolledSelectionList::renderHoleBackground(int y0, int y1, int a0,
                                                 int a1) {}
