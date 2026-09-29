#pragma once

#include <stdint.h>

#include <format>
#include <vector>

class DataLayer {
public:
    std::vector<uint8_t> data;

private:
    const int depthBits;
    const int depthBitsPlusFour;

public:
    DataLayer(int length, int depthBits);
    DataLayer(std::vector<uint8_t>& data, int depthBits);
    ~DataLayer();

    int get(int x, int y, int z);

    void set(int x, int y, int z, int val);
    bool isValid();
    void setAll(int br);
};
