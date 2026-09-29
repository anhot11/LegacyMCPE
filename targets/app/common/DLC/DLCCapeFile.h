#pragma once
#include <cstdint>
#include <string>

#include "DLCFile.h"

class DLCCapeFile : public DLCFile {
public:
    DLCCapeFile(const std::string& path);

    virtual void addData(std::uint8_t* pbData, std::uint32_t dataBytes);
};
