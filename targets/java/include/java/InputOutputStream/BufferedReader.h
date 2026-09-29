#pragma once

#include <string>

#include "Reader.h"

class BufferedReader : public Reader {
private:
    Reader* reader;
    char* buffer;

    unsigned int readMark;
    unsigned int bufferedMark;
    unsigned int bufferSize;
    bool eofReached;

    static const unsigned int BUFFER_MORE_AMOUNT = 64;
    void bufferMore();

public:
    BufferedReader(Reader* in);
    virtual ~BufferedReader();

    virtual void close();
    virtual int read();
    virtual int read(char cbuf[], unsigned int off, unsigned int len);
    std::string readLine();
};