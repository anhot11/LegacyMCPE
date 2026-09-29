#pragma once

#include <cstdint>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "app/common/App_structs.h"

class ArchiveFile;

class ArchiveManager {
public:
    ArchiveManager();

    void loadMediaArchive();
    ArchiveFile* getMediaArchive() const { return m_mediaArchive; }

    int getArchiveFileSize(const std::string& filename);
    bool hasArchiveFile(const std::string& filename);
    std::vector<uint8_t> getArchiveFile(const std::string& filename);

    // Texture Pack Data files (icon, banner, comparison shot & text)
    void addMemoryTPDFile(int iConfig, std::uint8_t* pbData,
                          unsigned int byteCount);
    void removeMemoryTPDFile(int iConfig);
    bool isFileInTPD(int iConfig);
    void getTPD(int iConfig, std::uint8_t** ppbData, unsigned int* pByteCount);
    int getTPDSize() { return m_MEM_TPD.size(); }
    int getTPConfigVal(char* pwchDataFile);

    void setRequiredTexturePackID(std::uint32_t texturePackId) {
        m_dwRequiredTexturePackID = texturePackId;
    }
    std::uint32_t getRequiredTexturePackID() const {
        return m_dwRequiredTexturePackID;
    }

    virtual void getFileFromTPD(eTPDFileType eType, std::uint8_t* pbData,
                                unsigned int byteCount, std::uint8_t** ppbData,
                                unsigned int* pByteCount) {
        *ppbData = nullptr;
        *pByteCount = 0;
    }

protected:
    ArchiveFile* m_mediaArchive;

private:
    std::unordered_map<int, PMEMDATA> m_MEM_TPD;
    std::mutex csMemTPDLock;
    std::uint32_t m_dwRequiredTexturePackID;
};
