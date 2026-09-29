#pragma once
// 4J Stu - Implements the Java InputStream but rather than writing directly to
// disc it writes through the save file

#include <stdint.h>

#include "java/InputOutputStream/InputStream.h"
#include "minecraft/world/level/storage/ConsoleSaveFileIO/ConsoleSavePath.h"

class ConsoleSaveFile;
class FileEntry;

class ConsoleSaveFileInputStream : public InputStream {
public:
    ConsoleSaveFileInputStream(ConsoleSaveFile* saveFile,
                               const ConsoleSavePath& file);
    ConsoleSaveFileInputStream(ConsoleSaveFile* saveFile, FileEntry* file);
    virtual int read();
    virtual int read(std::vector<uint8_t>& b);
    virtual int read(std::vector<uint8_t>& b, unsigned int offset,
                     unsigned int length);
    virtual void close();
    virtual int64_t skip(int64_t n) { return n; }

private:
    ConsoleSaveFile* m_saveFile;
    FileEntry* m_file;
};