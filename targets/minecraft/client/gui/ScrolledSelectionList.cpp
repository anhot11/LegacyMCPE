#include "ScrolledSelectionList.h"

#include "Button.h"

class Minecraft;

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

    // 4J Stu - Smoe default initialisers
    upId = 0;
    downId = 0;

    yDrag = 0.0f;
    yDragScale = 0.0f;
    yo = 0.0f;

    lastSelection = 0;
    lastSelectionTime = 0;

    renderSelection = false;
    _renderHeader = false;
    headerHeight = 0;
    // End
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
    int x0 = width / 2 - (92 + 16 + 2);
    int x1 = width / 2 + (92 + 16 + 2);

    int clickSlotPos = (y - y0 - headerHeight + (int)yo - 4);
    int slot = clickSlotPos / itemHeight;
    if (x >= x0 && x <= x1 && slot >= 0 && clickSlotPos >= 0 &&
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
    if (max < 0) max /= 2;
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
    // 4J Unused
}

void ScrolledSelectionList::renderHoleBackground(int y0, int y1, int a0,
                                                 int a1) {
    // 4J Unused
}
