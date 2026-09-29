#pragma once
#include <cstdint>
#include <string>

#include "DLCFile.h"

class DLCUIDataFile : public DLCFile {
private:
    std::uint8_t* m_pbData;
    std::uint32_t m_dataBytes;
    bool m_canDeleteData;

public:
    DLCUIDataFile(const std::string& path);
    ~DLCUIDataFile();

    using DLCFile::addData;
    using DLCFile::addParameter;

    virtual void addData(std::uint8_t* pbData, std::uint32_t dataBytes,
                         bool canDeleteData = false);
    virtual std::uint8_t* getData(std::uint32_t& dataBytes);
};
