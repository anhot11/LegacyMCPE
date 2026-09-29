#pragma once
#include "minecraft/client/renderer/Textures.h"

typedef std::vector<_TEXTURE_NAME> textureNameArray;
class ResourceLocation {
private:
    textureNameArray m_texture;
    std::string m_path;
    bool m_preloaded;

public:
    ResourceLocation() {
        m_preloaded = false;
        m_path = "";
    }

    ResourceLocation(_TEXTURE_NAME texture) {
        m_texture = {texture};
        m_preloaded = true;
    }

    ResourceLocation(std::string path) {
        m_path = path;
        m_preloaded = false;
    }

    ResourceLocation(std::vector<int> textures) {
        m_texture.resize(textures.size());
        for (unsigned int i = 0; i < textures.size(); ++i) {
            m_texture[i] = (_TEXTURE_NAME)textures[i];
        }
        m_preloaded = true;
    }

    ~ResourceLocation() = default;

    _TEXTURE_NAME getTexture() { return m_texture[0]; }

    _TEXTURE_NAME getTexture(int idx) { return m_texture[idx]; }

    int getTextureCount() { return m_texture.size(); }

    std::string getPath() { return m_path; }

    bool isPreloaded() { return m_preloaded; }
};
