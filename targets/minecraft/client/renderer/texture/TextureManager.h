#pragma once

#include <format>
#include <string>
#include <unordered_map>
#include <vector>

class Texture;
class Stitcher;
class TexturePack;
class BufferedImage;

class TextureManager {
private:
    static TextureManager* instance;

    int nextID;
    typedef std::unordered_map<std::string, int> stringIntMap;
    typedef std::unordered_map<int, Texture*> intTextureMap;
    intTextureMap idToTextureMap;
    stringIntMap stringToIDMap;

public:
    static void createInstance();
    static TextureManager* getInstance();

private:
    TextureManager();

public:
    int createTextureID();
    Texture* getTexture(const std::string& name);
    void registerName(const std::string& name, Texture* texture);
    void registerTexture(Texture* texture);
    void unregisterTexture(const std::string& name, Texture* texture);
    Stitcher* createStitcher(const std::string& name);
    std::vector<Texture*>* createTextures(
        const std::string& filename, bool mipmap);  // 4J added mipmap param

private:
    std::string getTextureNameFromPath(const std::string& filename);
    bool isAnimation(const std::string& filename, TexturePack* texturePack);

public:
    Texture* createTexture(const std::string& name, int mode, int width,
                           int height, int wrap, int format, int minFilter,
                           int magFilter, bool mipmap, BufferedImage* image);
    Texture* createTexture(const std::string& name, int mode, int width,
                           int height, int format,
                           bool mipmap);  // 4J Added mipmap param
};