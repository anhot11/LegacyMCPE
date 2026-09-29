#include "DLCGameRulesFile.h"

#include "DLCManager.h"
#include "app/common/DLC/DLCGameRules.h"

DLCGameRulesFile::DLCGameRulesFile(const std::string& path)
    : DLCGameRules(DLCManager::e_DLCType_GameRules, path) {
    m_pbData = nullptr;
    m_dataBytes = 0;
}

void DLCGameRulesFile::addData(std::uint8_t* pbData, std::uint32_t dataBytes) {
    m_pbData = pbData;
    m_dataBytes = dataBytes;
}

std::uint8_t* DLCGameRulesFile::getData(std::uint32_t& dataBytes) {
    dataBytes = m_dataBytes;
    return m_pbData;
}
