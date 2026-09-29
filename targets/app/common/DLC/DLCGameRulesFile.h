#pragma once
#include <cstdint>
#include <string>

#include "DLCGameRules.h"

class DLCGameRulesFile : public DLCGameRules {
private:
    std::uint8_t* m_pbData;
    std::uint32_t m_dataBytes;

public:
    DLCGameRulesFile(const std::string& path);

    virtual void addData(std::uint8_t* pbData, std::uint32_t dataBytes);
    virtual std::uint8_t* getData(std::uint32_t& dataBytes);
};
