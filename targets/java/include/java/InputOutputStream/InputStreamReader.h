#pragma once

#include "DataInputStream.h"
#include "Reader.h"

class InputStream;
class DataInputStream;

class InputStreamReader : public Reader {
private:
    DataInputStream* stream;

public:
    InputStreamReader(InputStream* in);

    virtual void close();
    virtual int read();
    virtual int read(char cbuf[], unsigned int offset, unsigned int length);
};