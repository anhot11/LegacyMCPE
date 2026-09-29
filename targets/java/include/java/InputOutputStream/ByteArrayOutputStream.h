#pragma once

#include <cstdint>
#include <vector>

#include "OutputStream.h"

class ByteArrayOutputStream : public OutputStream {
    // Note - when actually implementing, std::vector<uint8_t> will need to grow
    // as data is written
public:
    std::vector<uint8_t> buf;  // The buffer where data is stored.

protected:
    unsigned int count;  // The number of valid bytes in the buffer.

public:
    ByteArrayOutputStream();
    ByteArrayOutputStream(unsigned int size);
    virtual ~ByteArrayOutputStream();

    virtual void flush() {}
    virtual void write(unsigned int b);
    virtual void write(const std::vector<uint8_t>& b);
    virtual void write(const std::vector<uint8_t>& b, unsigned int offset,
                       unsigned int length);
    virtual void close();
    virtual std::vector<uint8_t> toByteArray();

    void reset() { count = 0; }
    unsigned int size() { return count; }
};