#pragma once

#include <cstdint>
#include <string>
#include <vector>

class DataOutput {
public:
    virtual void write(unsigned int b) = 0;
    virtual void write(const std::vector<uint8_t>& b) = 0;
    virtual void write(const std::vector<uint8_t>& b, unsigned int offset,
                       unsigned int length) = 0;
    virtual void writeByte(uint8_t a) = 0;
    virtual void writeDouble(double a) = 0;
    virtual void writeFloat(float a) = 0;
    virtual void writeInt(int a) = 0;
    virtual void writeLong(int64_t a) = 0;
    virtual void writeShort(short a) = 0;
    virtual void writeBoolean(bool v) = 0;
    virtual void writeChar(char v) = 0;
    virtual void writeChars(const std::string& s) = 0;
    virtual void writeUTF(const std::string& a) = 0;
    virtual void writePlayerUID(unsigned long long player) = 0;  // 4J Added
};