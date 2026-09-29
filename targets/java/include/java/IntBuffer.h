#pragma once

#include <vector>

#include "Buffer.h"

class IntBuffer : public Buffer {
private:
    int* buffer;

public:
    IntBuffer(unsigned int capacity);
    IntBuffer(unsigned int capacity, int* backingArray);
    virtual ~IntBuffer();

    virtual IntBuffer* flip();
    int get(unsigned int index);
    int* getBuffer();
    IntBuffer* put(std::vector<int>* inputArray, unsigned int offset,
                   unsigned int length);
    IntBuffer* put(std::vector<int>& inputArray);
    IntBuffer* put(int i);
};