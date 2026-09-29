#pragma once
// 4J Stu - Implements the Java InputStream but rather than writing directly to
// disc it writes through the save file

#include <stdint.h>

#include "java/InputOutputStream/OutputStream.h"
#include "minecraft/world/level/storage/ConsoleSaveFileIO/ConsoleSavePath.h"

class ConsoleSaveFile;
class FileEntry;

class ConsoleSaveFileOutputStream : public OutputStream {
public:
    ConsoleSaveFileOutputStream(ConsoleSaveFile* saveFile,
                                const ConsoleSavePath& file);
    ConsoleSaveFileOutputStream(ConsoleSaveFile* saveFile, FileEntry* file);
    virtual void write(unsigned int b);
    virtual void write(const std::vector<uint8_t>& b);
    virtual void write(const std::vector<uint8_t>& b, unsigned int offset,
                       unsigned int length);
    virtual void close();
    virtual void flush() {}

private:
    ConsoleSaveFile* m_saveFile;
    FileEntry* m_file;
};