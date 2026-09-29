#pragma once
#include <cstdio>

#include "ZonedChunkStorage.h"
#include "nbt/CompoundTag.h"
#include "nbt/NbtIo.h"

class NbtSlotFile {
private:
    static std::vector<uint8_t> READ_BUFFER;

    static const int FILE_HEADER_SIZE = 1024;
    static const int MAGIC_NUMBER = 0x13737001;

    static const int FILE_SLOT_HEADER_SIZE = 12;
    static const int FILE_SLOT_SIZE = 500;

    std::FILE* raf;
    std::vector<int>** fileSlotMap;
    int fileSlotMapLength;
    std::vector<int> freeFileSlots;
    int totalFileSlots;
    static int64_t largest;

public:
    NbtSlotFile(File file);

private:
    void readHeader();
    void writeHeader();
    void seekSlotHeader(int fileSlot);
    void seekSlot(int fileSlot);

public:
    std::vector<CompoundTag*>* readAll(int slot);

private:
    std::vector<int>* toReplace;

    int getFreeSlot();

public:
    void replaceSlot(int slot, std::vector<CompoundTag*>* tags);
    void close();
};
