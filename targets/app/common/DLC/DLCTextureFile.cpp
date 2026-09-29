#include "DLCTextureFile.h"

#include "DLCManager.h"
#include "app/common/DLC/DLCFile.h"

DLCTextureFile::DLCTextureFile(const std::string& path)
    : DLCFile(DLCManager::e_DLCType_Texture, path) {
    m_bIsAnim = false;
    m_animString = "";

    m_pbData = nullptr;
    m_dataBytes = 0;
}

void DLCTextureFile::addData(std::uint8_t* pbData, std::uint32_t dataBytes) {
    // app.AddMemoryTextureFile(m_path,pbData,dwBytes);
    m_pbData = pbData;
    m_dataBytes = dataBytes;
}

std::uint8_t* DLCTextureFile::getData(std::uint32_t& dataBytes) {
    dataBytes = m_dataBytes;
    return m_pbData;
}

void DLCTextureFile::addParameter(DLCManager::EDLCParameterType type,
                                  const std::string& value) {
    switch (type) {
        case DLCManager::e_DLCParamType_Anim:
            m_animString = value;
            m_bIsAnim = true;

            break;
        default:
            break;
    }
}

std::string DLCTextureFile::getParameterAsString(
    DLCManager::EDLCParameterType type) {
    switch (type) {
        case DLCManager::e_DLCParamType_Anim:
            return m_animString;
        default:
            return "";
    }
}

bool DLCTextureFile::getParameterAsBool(DLCManager::EDLCParameterType type) {
    switch (type) {
        case DLCManager::e_DLCParamType_Anim:
            return m_bIsAnim;
        default:
            return false;
    }
}
