#pragma once
#include <string>

class Font;

class GuiComponent {
protected:
    float blitOffset;

public:
    static void fill(int x0, int y0, int x1, int y1, int col);
    static void fillGradient(int x0, int y0, int x1, int y1, int col1, int col2, float z = 0.0f);

protected:
    void hLine(int x0, int x1, int y, int col);
    void vLine(int x, int y0, int y1, int col);

public:
    GuiComponent();  // 4J added
    void drawCenteredString(Font* font, const std::string& str, int x, int y,
                            int color);
    void drawString(Font* font, const std::string& str, int x, int y,
                    int color);
    void blit(int x, int y, int sx, int sy, int w, int h);
    void blit(int x, int y, int sx, int sy, int w, int h, int tw, int th);
};
