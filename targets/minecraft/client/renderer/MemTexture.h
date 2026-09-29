#pragma once

#include <cstdint>
#include <string>

class BufferedImage;
class MemTextureProcessor;

class MemTexture {
public:
    BufferedImage* loadedImage;
    int count;
    int id;
    bool isLoaded;
    int ticksSinceLastUse;
    static const int UNUSED_TICKS_TO_FREE = 20;

    MemTexture(const std::string& _name, std::uint8_t* pbData,
               std::uint32_t dataBytes, MemTextureProcessor* processor);
    ~MemTexture();
};
