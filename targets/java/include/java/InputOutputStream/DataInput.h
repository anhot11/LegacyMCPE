#pragma once

#include <cstdint>
#include <string>
#include <vector>

class DataInput {
public:
    virtual int read() = 0;
    virtual int read(std::vector<uint8_t>& b) = 0;
    virtual int read(std::vector<uint8_t>& b, unsigned int offset,
                     unsigned int length) = 0;
    virtual bool readBoolean() = 0;
    virtual uint8_t readByte() = 0;
    virtual unsigned char readUnsignedByte() = 0;
    virtual bool readFully(std::vector<uint8_t>& a) = 0;
    virtual double readDouble() = 0;
    virtual float readFloat() = 0;
    virtual int readInt() = 0;
    virtual int64_t readLong() = 0;
    virtual short readShort() = 0;
    virtual char readChar() = 0;
    virtual std::string readUTF() = 0;
    virtual unsigned long long readPlayerUID() = 0;  // 4J Added
    virtual int skipBytes(int n) = 0;
};
