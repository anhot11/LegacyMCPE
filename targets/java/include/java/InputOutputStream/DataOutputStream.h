#pragma once
// 4J Stu - Represents Java standard library class (although we miss out an
// intermediate inheritance class that we don't care about)

#include <stdint.h>

#include <string>

#include "DataOutput.h"
#include "OutputStream.h"

class DataOutputStream : public OutputStream, public DataOutput {
private:
    OutputStream* stream;

protected:
    int written;  // The number of bytes written to the data output stream so
                  // far.

public:
    DataOutputStream(OutputStream* out);

    // 4J Stu Added
    void deleteChildStream();

    virtual void write(unsigned int b);
    virtual void write(const std::vector<uint8_t>& b);
    virtual void write(const std::vector<uint8_t>& b, unsigned int offset,
                       unsigned int length);
    virtual void close();
    virtual void writeByte(uint8_t a);
    virtual void writeDouble(double a);
    virtual void writeFloat(float a);
    virtual void writeInt(int a);
    virtual void writeLong(int64_t a);
    virtual void writeShort(short a);
    virtual void writeUnsignedShort(unsigned short a);
    virtual void writeChar(char a);
    virtual void writeChars(const std::string& a);
    virtual void writeBoolean(bool b);
    virtual void writeUTF(const std::string& a);
    virtual void writePlayerUID(unsigned long long player);
    virtual void flush();
};