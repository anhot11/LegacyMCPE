#pragma once

#include <cstdint>
#include <string>
#include <vector>

// 4J Stu - Represents Java standard lib abstract

class InputStream {
public:
    virtual ~InputStream() {}

    virtual int read() = 0;
    virtual int read(std::vector<uint8_t>& b) = 0;
    virtual int read(std::vector<uint8_t>& b, unsigned int offset,
                     unsigned int length) = 0;
    virtual void close() = 0;
    virtual int64_t skip(int64_t n) = 0;

    static InputStream* getResourceAsStream(const std::string& fileName);
};