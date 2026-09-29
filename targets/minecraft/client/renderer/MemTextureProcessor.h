#pragma once
class BufferedImage;

class MemTextureProcessor {
public:
    virtual ~MemTextureProcessor() {}
    virtual BufferedImage* process(BufferedImage* read) = 0;
};
