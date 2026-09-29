#pragma once
#include <cstdint>
#include <string>

#include "DLCManager.h"

class DLCFile {
protected:
    DLCManager::EDLCType m_type;
    std::string m_path;
    std::uint32_t m_dwSkinId;

public:
    DLCFile(DLCManager::EDLCType type, const std::string& path);
    virtual ~DLCFile() {}

    DLCManager::EDLCType getType() { return m_type; }
    std::string getPath() { return m_path; }
    std::uint32_t getSkinID() { return m_dwSkinId; }

    virtual void addData(std::uint8_t* pbData, std::uint32_t dataBytes) {}
    virtual std::uint8_t* getData(std::uint32_t& dataBytes) {
        dataBytes = 0;
        return nullptr;
    }
    virtual void addParameter(DLCManager::EDLCParameterType type,
                              const std::string& value) {}

    virtual std::string getParameterAsString(
        DLCManager::EDLCParameterType type) {
        return "";
    }
    virtual bool getParameterAsBool(DLCManager::EDLCParameterType type) {
        return false;
    }
};
