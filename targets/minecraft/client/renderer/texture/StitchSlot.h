#pragma once

#include <format>
#include <string>
#include <vector>

class TextureHolder;

class StitchSlot {
private:
    const int originX;
    const int originY;

    const int width;
    const int height;
    std::vector<StitchSlot*>* subSlots;
    TextureHolder* textureHolder;

public:
    StitchSlot(int originX, int originY, int width, int height);

    TextureHolder* getHolder();
    int getX();
    int getY();
    bool add(TextureHolder* textureHolder);
    void collectAssignments(std::vector<StitchSlot*>* result);

    //@Override
    std::string toString();
};