#pragma once
#include <cstdint>
#include <string>

#include "DLCFile.h"

class ColourTable;

class DLCColourTableFile : public DLCFile {
private:
    ColourTable* m_colourTable;

public:
    DLCColourTableFile(const std::string& path);
    ~DLCColourTableFile();

    virtual void addData(std::uint8_t* pbData, std::uint32_t dataBytes);

    ColourTable* getColourTable() { return m_colourTable; }
};
