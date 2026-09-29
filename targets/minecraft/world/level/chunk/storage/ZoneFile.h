#pragma once
#include <cstdio>

#include "NbtSlotFile.h"
#include "ZoneIo.h"
#include "ZonedChunkStorage.h"

class ZoneFile {
public:
    static const int FILE_HEADER_SIZE = 1024 * 4;

private:
    static const int MAGIC_NUMBER = 0x13737000;

    static const int slotsLength;

    std::vector<short> slots;
    short slotCount;

public:
    int64_t lastUse;

private:
    std::FILE* channel;

public:
    int64_t key;
    File file;

    NbtSlotFile* entityFile;

    ZoneFile(int64_t key, File file, File entityFile);
    ~ZoneFile();

    void readHeader();

    void writeHeader();

public:
    void close();
    ZoneIo* getZoneIo(int slot);
    bool containsSlot(int slot);
};
