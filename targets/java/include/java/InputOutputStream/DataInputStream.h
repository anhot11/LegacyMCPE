#pragma once
// 4J Stu - Represents Java standard library class (although we miss out an
// intermediate inheritance class that we don't care about)

#include <cstdint>
#include <string>
#include <vector>

#include "DataInput.h"
#include "InputStream.h"

class DataInputStream : public InputStream, public DataInput {
private:
    InputStream* stream;

public:
    DataInputStream(InputStream* in);
    virtual int read();
    virtual int read(std::vector<uint8_t>& b);
    virtual int read(std::vector<uint8_t>& b, unsigned int offset,
                     unsigned int length);
    virtual void close();
    virtual bool readBoolean();
    virtual uint8_t readByte();
    virtual unsigned char readUnsignedByte();
    virtual char readChar();
    virtual bool readFully(std::vector<uint8_t>& b);
    virtual bool readFully(std::vector<char>& b);
    virtual double readDouble();
    virtual float readFloat();
    virtual int readInt();
    virtual int64_t readLong();
    virtual short readShort();
    virtual unsigned short readUnsignedShort();
    virtual std::string readUTF();
    void deleteChildStream();
    virtual int readUTFChar();
    virtual unsigned long long readPlayerUID();  // 4J Added
    virtual int64_t skip(int64_t n);
    virtual int skipBytes(int n);
};