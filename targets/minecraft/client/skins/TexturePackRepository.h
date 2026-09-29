#pragma once

#include <cstdint>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>

#include "TexturePack.h"
#include "java/File.h"

class Minecraft;
class DLCPack;
class TexturePack;

class TexturePackRepository {
public:
    static constexpr std::uint32_t DEFAULT_TEXTURE_PACK_ID = 0;
    static constexpr std::uint32_t FOLDER_TEST_TEXTURE_PACK_ID = 1;
    static constexpr std::uint32_t DLC_TEST_TEXTURE_PACK_ID = 2;

private:
    static TexturePack* DEFAULT_TEXTURE_PACK;
    TexturePack* m_dummyTexturePack;
    TexturePack* m_dummyDLCTexturePack;

    Minecraft* minecraft;
    File workDir;
    File multiplayerDir;
    std::vector<TexturePack*>* texturePacks;
    std::vector<TexturePack*> m_texturePacksToDelete;

    std::unordered_map<std::uint32_t, TexturePack*> cacheById;

    TexturePack* selected;
    TexturePack* lastSelected;
    bool usingWeb;
    static const int MAX_WEB_FILESIZE = 10 * 1000 * 1000;  // 10 Megabytes

public:
    TexturePackRepository(File workingDirectory, Minecraft* minecraft);
    void addDebugPacks();

private:
    void createWorkingDirecoryUnlessExists();

public:
    bool selectSkin(TexturePack* skin);

    void selectWebSkin(const std::string& url);

private:
    void downloadWebSkin(const std::string& url, File file);

public:
    bool isUsingWebSkin();
    void resetWebSkin();
    void updateList();

private:
    std::string getIdOrNull(File file);
    std::vector<File> getWorkDirContents();

public:
    std::vector<TexturePack*>* getAll();

    TexturePack* getSelected();
    bool shouldPromptForWebSkin();
    bool canUseWebSkin();
    bool isUsingDefaultSkin() {
        return selected == DEFAULT_TEXTURE_PACK;
    }  // 4J Added
    TexturePack* getDefault() { return DEFAULT_TEXTURE_PACK; }  // 4J Added

    std::vector<std::pair<std::uint32_t, std::string> >*
    getTexturePackIdNames();
    bool selectTexturePackById(std::uint32_t id);       // 4J Added
    TexturePack* getTexturePackById(std::uint32_t id);  // 4J Added

    TexturePack* addTexturePackFromDLC(DLCPack* dlcPack, std::uint32_t id);
    void clearInvalidTexturePacks();
    void updateUI();
    bool needsUIUpdate();

private:
    void removeTexturePackById(std::uint32_t id);

public:
    unsigned int getTexturePackCount();
    TexturePack* getTexturePackByIndex(unsigned int index);
    unsigned int getTexturePackIndex(std::uint32_t id);
};
