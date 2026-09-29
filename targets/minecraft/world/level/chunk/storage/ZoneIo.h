#pragma once
#include <cstdio>

#include "ZonedChunkStorage.h"

class ByteBuffer;

class ZoneIo {
private:
    std::FILE* channel;
    int64_t pos;

public:
    ZoneIo(std::FILE* channel, int64_t pos);
    void write(std::vector<uint8_t> bb, int size);
    void write(ByteBuffer* bb, int size);
    ByteBuffer* read(int size);

    void flush();
};
