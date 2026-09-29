#include "FileTexturePack.h"

#include "java/File.h"
#include "minecraft/client/skins/AbstractTexturePack.h"

class TexturePack;

FileTexturePack::FileTexturePack(std::uint32_t id, File* file,
                                 TexturePack* fallback)
    : AbstractTexturePack(id, file, file->getName(), fallback) {
    // 4J Stu - These calls need to be in the most derived version of the class
    loadIcon();
    loadName();
    loadDescription();
}

void FileTexturePack::unload(Textures* textures) {}

InputStream* FileTexturePack::getResourceImplementation(
    const std::string& name)  // throws IOException
{
    return nullptr;
}

bool FileTexturePack::hasFile(const std::string& name) { return false; }

void FileTexturePack::loadZipFile()  // throws IOException
{}

bool FileTexturePack::isTerrainUpdateCompatible() { return false; }
