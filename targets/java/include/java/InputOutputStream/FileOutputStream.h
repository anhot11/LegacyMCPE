#pragma once
// 4J Stu - Represents Java standard lib abstract

#include <stdint.h>

#include <cstdio>

#include "OutputStream.h"

class File;

class FileOutputStream : public OutputStream {
public:
    FileOutputStream(const File& file);
    virtual ~FileOutputStream();
    virtual void write(unsigned int b);
    virtual void write(const std::vector<uint8_t>& b);
    virtual void write(const std::vector<uint8_t>& b, unsigned int offset,
                       unsigned int length);
    virtual void close();
    virtual void flush();

private:
    std::FILE* m_fileHandle;
};
