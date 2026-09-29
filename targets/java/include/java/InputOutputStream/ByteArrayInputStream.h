#pragma once
// 4J Stu - Represents Java standard library class

#include <cstdint>
#include <vector>

#include "InputStream.h"

class ByteArrayInputStream : public InputStream {
protected:
    std::vector<uint8_t> buf;  // An array of bytes that was provided by the
                               // creator of the stream.
    unsigned int count;  // The index one greater than the last valid character
                         // in the input stream buffer.
    unsigned int mark;   // The currently marked position in the stream.
    unsigned int pos;  // The index of the next character to read from the input
                       // stream buffer.

public:
    ByteArrayInputStream(std::vector<uint8_t>& buf, unsigned int offset,
                         unsigned int length);
    ByteArrayInputStream(std::vector<uint8_t>& buf);
    // takes ownership of the vector
    ByteArrayInputStream(std::vector<uint8_t>&& buf);
    virtual ~ByteArrayInputStream();
    virtual int read();
    virtual int read(std::vector<uint8_t>& b);
    virtual int read(std::vector<uint8_t>& b, unsigned int offset,
                     unsigned int length);
    virtual void close();
    virtual int64_t skip(int64_t n);

    // 4J Stu Added - Sometimes we don't want to delete the data on destroying
    // this
    void reset() {
        buf = std::vector<uint8_t>();
        count = 0;
        mark = 0;
        pos = 0;
    }
};
