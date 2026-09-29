#pragma once
#include <string>

class BufferedImage;
class HttpTextureProcessor;

class HttpTexture {
public:
    BufferedImage* loadedImage;
    int count;
    int id;
    bool isLoaded;

    HttpTexture(const std::string& _url, HttpTextureProcessor* processor);
};