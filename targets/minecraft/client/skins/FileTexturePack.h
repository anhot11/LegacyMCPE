#pragma once
#include <cstdint>
#include <string>

#include "AbstractTexturePack.h"

// class ZipFile;
class BufferedImage;
class File;
class Textures;
class TexturePack;

class FileTexturePack : public AbstractTexturePack {
private:
    // ZipFile *zipFile;

public:
    FileTexturePack(std::uint32_t id, File* file, TexturePack* fallback);

    //@Override
    void unload(Textures* textures);

protected:
    InputStream* getResourceImplementation(
        const std::string& name);  // throws IOException

public:
    //@Override
    bool hasFile(const std::string& name);

private:
    void loadZipFile();  // throws IOException

public:
    bool isTerrainUpdateCompatible();
};
