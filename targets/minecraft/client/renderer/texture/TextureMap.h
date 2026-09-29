#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "minecraft/world/IconRegister.h"

class StitchedTexture;
class Texture;
class BufferedImage;
class Icon;

class TextureMap : public IconRegister {
public:
    static const std::string NAME_MISSING_TEXTURE;

private:
    const int iconType;

    const std::string name;
    const std::string path;
    const std::string extension;

    bool m_mipMap;

    typedef std::unordered_map<std::string, StitchedTexture*>
        stringStitchedTextureMap;
    stringStitchedTextureMap
        texturesByName;  //  = new HashMap<String, StitchedTexture>();
    BufferedImage* missingTexture;  // = new BufferedImage(64, 64,
                                    // BufferedImage.TYPE_INT_ARGB);
    StitchedTexture* missingPosition;
    Texture* stitchResult;
    std::vector<StitchedTexture*>
        animatedTextures;  // = new ArrayList<StitchedTexture>();

    stringStitchedTextureMap
        texturesToRegister;  // = new HashMap<String, StitchedTexture>();

public:
    TextureMap(int type, const std::string& name, const std::string& path,
               BufferedImage* missingTexture, bool mipMap = false);

    void stitch();
    StitchedTexture* getTexture(const std::string& name);
    void cycleAnimationFrames();
    Texture* getStitchedTexture();

    // 4J Stu - register is a reserved keyword in C++
    Icon* registerIcon(const std::string& name);

    int getIconType();
    Icon* getMissingIcon();
};