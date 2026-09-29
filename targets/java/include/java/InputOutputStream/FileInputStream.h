#pragma once
// 4J Stu - Represents Java standard library class

#include <cstdint>
#include <cstdio>

#include "InputStream.h"

class File;

class FileInputStream : public InputStream {
public:
    FileInputStream(const File& file);
    virtual ~FileInputStream();
    virtual int read();
    virtual int read(std::vector<uint8_t>& b);
    virtual int read(std::vector<uint8_t>& b, unsigned int offset,
                     unsigned int length);
    virtual void close();
    virtual int64_t skip(int64_t n);

private:
    std::FILE* m_fileHandle;
};
