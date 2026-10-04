#include "StubStorage.h"

#include <stdlib.h>

#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "minecraft/util/Log.h"
#include "platform/fs/fs.h"

namespace platform_internal {
IPlatformStorage& PlatformStorage_get() {
    static StubStorage instance;
    return instance;
}
}  // namespace platform_internal

static std::filesystem::path getSavesRoot() {
    std::filesystem::path base = PlatformFilesystem.getBasePath();
    std::filesystem::path savesDir = base / "saves";
    std::error_code ec;
    if (!std::filesystem::exists(savesDir, ec)) {
        std::filesystem::create_directories(savesDir, ec);
    }
    return savesDir;
}

static std::string sanitizeSaveName(const std::string& name) {
    std::string safe = name.empty() ? "World" : name;
    for (char& c : safe) {
        if (c == '/' || c == '\\' || c == ':' || c == '*' || c == '?' || c == '"' || c == '<' || c == '>' || c == '|') {
            c = '_';
        }
    }
    return safe;
}

static XMARKETPLACE_CONTENTOFFER_INFO s_dummyOffer = {};
static XCONTENT_DATA s_dummyContentData = {};

StubStorage::StubStorage() : m_allocatedBuffer(nullptr), m_allocatedBufferSize(0), m_pStringTable(nullptr) {}

void StubStorage::Tick(void) {}

StubStorage::EMessageResult StubStorage::RequestMessageBox(
    unsigned int uiTitle, unsigned int uiText, unsigned int* uiOptionA,
    unsigned int uiOptionC, unsigned int pad,
    std::function<int(int, const StubStorage::EMessageResult)> callback,
    C4JStringTable* pStringTable, char* pwchFormatString,
    unsigned int focusButton) {
    return EMessage_ResultAccept;
}

StubStorage::EMessageResult StubStorage::GetMessageBoxResult() {
    return EMessage_Undefined;
}

bool StubStorage::SetSaveDevice(std::function<int(const bool)> callback,
                                bool bForceResetOfSaveDevice) {
    return true;
}

void StubStorage::Init(unsigned int uiSaveVersion,
                       const char* pwchDefaultSaveName, char* pszSavePackName,
                       int iMinimumSaveSize,
                       std::function<int(const ESavingMessage, int)> callback,
                       const char* szGroupID) {}
void StubStorage::LoadFromDisk(const std::string& title) {
    if (title.empty()) return;
    std::string safeName = sanitizeSaveName(title);
    std::filesystem::path worldDir = getSavesRoot() / safeName;
    std::error_code ec;
    if (!std::filesystem::exists(worldDir, ec)) {
        std::filesystem::path alt = std::filesystem::path("/sdcard/LegacyMCPE/saves") / safeName;
        if (std::filesystem::exists(alt, ec)) {
            worldDir = alt;
        } else {
            bool found = false;
            std::filesystem::path roots[] = { getSavesRoot(), std::filesystem::path("/sdcard/LegacyMCPE/saves") };
            for (const auto& root : roots) {
                if (found) break;
                if (!std::filesystem::exists(root, ec)) continue;
                for (const auto& dir : std::filesystem::directory_iterator(root, ec)) {
                    if (dir.is_directory()) {
                        std::filesystem::path infoPath = dir.path() / "info.txt";
                        if (std::filesystem::exists(infoPath, ec)) {
                            std::ifstream fInfo(infoPath);
                            std::string line;
                            if (std::getline(fInfo, line)) {
                                if (line == title || sanitizeSaveName(line) == safeName) {
                                    worldDir = dir.path();
                                    found = true;
                                    break;
                                }
                            }
                        }
                        if (!found && dir.path().filename().string() == title) {
                            worldDir = dir.path();
                            found = true;
                            break;
                        }
                    }
                }
            }
            if (!found) return;
        }
    }

    std::filesystem::path datPath = worldDir / "savegame.dat";
    if (std::filesystem::exists(datPath, ec)) {
        std::ifstream f(datPath, std::ios::binary | std::ios::ate);
        if (f.is_open()) {
            std::streamsize sz = f.tellg();
            f.seekg(0, std::ios::beg);
            m_saveData.resize((size_t)sz);
            if (sz > 0) {
                f.read(reinterpret_cast<char*>(m_saveData.data()), sz);
            }
        }
    }

    m_subfiles.clear();
    for (const auto& entry : std::filesystem::directory_iterator(worldDir, ec)) {
        if (entry.is_regular_file()) {
            std::string fname = entry.path().filename().string();
            unsigned int regIdx = 0;
            if (sscanf(fname.c_str(), "subfile_%u.bin", &regIdx) == 1) {
                std::ifstream sf(entry.path(), std::ios::binary | std::ios::ate);
                if (sf.is_open()) {
                    std::streamsize ssz = sf.tellg();
                    sf.seekg(0, std::ios::beg);
                    SubfileData sub;
                    sub.regionIndex = regIdx;
                    sub.data.resize((size_t)ssz);
                    if (ssz > 0) {
                        sf.read(reinterpret_cast<char*>(sub.data.data()), ssz);
                    }
                    m_subfiles.push_back(std::move(sub));
                }
            }
        }
    }
}

void StubStorage::ResetSaveData() {
    m_currentSaveTitle.clear();
    m_saveData.clear();
    m_subfiles.clear();
    if (m_allocatedBuffer) {
        free(m_allocatedBuffer);
        m_allocatedBuffer = nullptr;
    }
    m_allocatedBufferSize = 0;
    m_actualSaveDataSize = 0;
}

void StubStorage::SetDefaultSaveNameForKeyboardDisplay(
    const char* pwchDefaultSaveName) {}

void StubStorage::SetSaveTitle(const char* pwchDefaultSaveName) {
    if (pwchDefaultSaveName && pwchDefaultSaveName[0] != '\0') {
        m_currentSaveTitle = pwchDefaultSaveName;
    } else if (m_currentSaveTitle.empty()) {
        m_currentSaveTitle = "World";
    }
    m_currentSaveTitle = sanitizeSaveName(m_currentSaveTitle);
    if (m_saveData.empty()) {
        LoadFromDisk(m_currentSaveTitle);
    }
}

bool StubStorage::GetSaveUniqueNumber(int* piVal) {
    if (piVal) *piVal = 0;
    return true;
}
bool StubStorage::GetSaveUniqueFilename(char* pszName) {
    if (pszName) pszName[0] = '\0';
    return true;
}
void StubStorage::SetSaveUniqueFilename(char* szFilename) {}
void StubStorage::SetState(ESaveGameControlState eControlState,
                           std::function<int(const bool)> callback) {}
void StubStorage::SetSaveDisabled(bool bDisable) {}
bool StubStorage::GetSaveDisabled(void) { return false; }

unsigned int StubStorage::GetSaveSize() {
    if (m_saveData.empty() && !m_currentSaveTitle.empty()) {
        LoadFromDisk(m_currentSaveTitle);
    }
    return (unsigned int)m_saveData.size();
}

void StubStorage::GetSaveData(void* pvData, unsigned int* puiBytes) {
    if (m_saveData.empty() && !m_currentSaveTitle.empty()) {
        LoadFromDisk(m_currentSaveTitle);
    }
    if (pvData && !m_saveData.empty()) {
        memcpy(pvData, m_saveData.data(), m_saveData.size());
    }
    if (puiBytes) {
        *puiBytes = (unsigned int)m_saveData.size();
    }
}

void* StubStorage::AllocateSaveData(unsigned int uiBytes) {
    if (m_allocatedBuffer) {
        free(m_allocatedBuffer);
        m_allocatedBuffer = nullptr;
    }
    m_allocatedBufferSize = uiBytes;
    m_actualSaveDataSize = 0;
    m_allocatedBuffer = (uint8_t*)malloc(uiBytes);
    return m_allocatedBuffer;
}

void StubStorage::SetSaveImages(std::uint8_t* pbThumbnail,
                                unsigned int thumbnailBytes,
                                std::uint8_t* pbImage, unsigned int imageBytes,
                                std::uint8_t* pbTextData,
                                unsigned int textDataBytes) {}

StubStorage::ESaveGameState StubStorage::SaveSaveData(
    std::function<int(const bool)> callback) {
    unsigned int writeSize = (m_actualSaveDataSize > 0 && m_actualSaveDataSize <= m_allocatedBufferSize)
                             ? m_actualSaveDataSize : m_allocatedBufferSize;
    if (m_allocatedBuffer && writeSize > 0) {
        m_saveData.assign(m_allocatedBuffer, m_allocatedBuffer + writeSize);
    }
    if (!m_saveData.empty()) {
        std::string safeName = sanitizeSaveName(m_currentSaveTitle);
        std::filesystem::path worldDir = getSavesRoot() / safeName;
        std::error_code ec;
        std::filesystem::create_directories(worldDir, ec);

        std::filesystem::path datPath = worldDir / "savegame.dat";
        std::ofstream f(datPath, std::ios::binary | std::ios::trunc);
        if (f.is_open()) {
            f.write(reinterpret_cast<const char*>(m_saveData.data()), m_saveData.size());
            f.close();
        }

        std::filesystem::path infoPath = worldDir / "info.txt";
        std::ofstream info(infoPath, std::ios::trunc);
        if (info.is_open()) {
            info << m_currentSaveTitle << "\n";
            info.close();
        }
    }
    if (callback) {
        callback(true);
    }
    return ESaveGame_Idle;
}

void StubStorage::CopySaveDataToNewSave(std::uint8_t* pbThumbnail,
                                        unsigned int cbThumbnail,
                                        char* wchNewName,
                                        std::function<int(bool)> callback) {}
void StubStorage::SetSaveDeviceSelected(unsigned int uiPad, bool bSelected) {}
bool StubStorage::GetSaveDeviceSelected(unsigned int iPad) { return true; }

StubStorage::ESaveGameState StubStorage::DoesSaveExist(bool* pbExists) {
    if (pbExists) {
        std::string safeName = sanitizeSaveName(m_currentSaveTitle);
        std::filesystem::path datPath = getSavesRoot() / safeName / "savegame.dat";
        std::error_code ec;
        *pbExists = std::filesystem::exists(datPath, ec);
    }
    return ESaveGame_Idle;
}
bool StubStorage::EnoughSpaceForAMinSaveGame() { return true; }
void StubStorage::SetSaveMessageVPosition(float fY) {}
StubStorage::ESaveGameState StubStorage::GetSavesInfo(
    int iPad,
    std::function<int(SAVE_DETAILS* pSaveDetails, const bool)> callback,
    char* pszSavePackName) {
    return ESaveGame_Idle;
}
PSAVE_DETAILS StubStorage::ReturnSavesInfo() { return nullptr; }
void StubStorage::ClearSavesInfo() {}
StubStorage::ESaveGameState StubStorage::LoadSaveDataThumbnail(
    PSAVE_INFO pSaveInfo,
    std::function<int(std::uint8_t* thumbnailData, unsigned int thumbnailBytes)>
        callback) {
    return ESaveGame_Idle;
}
void StubStorage::GetSaveCacheFileInfo(unsigned int fileIndex,
                                       XCONTENT_DATA& xContentData) {
    memset(&xContentData, 0, sizeof(xContentData));
}
void StubStorage::GetSaveCacheFileInfo(unsigned int fileIndex,
                                       std::uint8_t** ppbImageData,
                                       unsigned int* pImageBytes) {
    if (ppbImageData) *ppbImageData = nullptr;
    if (pImageBytes) *pImageBytes = 0;
}
StubStorage::ESaveGameState StubStorage::LoadSaveData(
    PSAVE_INFO pSaveInfo, std::function<int(const bool, const bool)> callback) {
    return ESaveGame_Idle;
}
StubStorage::ESaveGameState StubStorage::DeleteSaveData(
    PSAVE_INFO pSaveInfo, std::function<int(const bool)> callback) {
    return ESaveGame_Idle;
}
void StubStorage::RegisterMarketplaceCountsCallback(
    std::function<int(StubStorage::DLC_TMS_DETAILS*, int)> callback) {}
void StubStorage::SetDLCPackageRoot(char* pszDLCRoot) {}
StubStorage::EDLCStatus StubStorage::GetDLCOffers(
    int iPad, std::function<int(int, std::uint32_t, int)> callback,
    std::uint32_t dwOfferTypesBitmask) {
    return EDLC_NoOffers;
}
unsigned int StubStorage::CancelGetDLCOffers() { return 0; }
void StubStorage::ClearDLCOffers() {}
XMARKETPLACE_CONTENTOFFER_INFO& StubStorage::GetOffer(unsigned int dw) {
    return s_dummyOffer;
}
int StubStorage::GetOfferCount() { return 0; }
unsigned int StubStorage::InstallOffer(int iOfferIDC,
                                       std::uint64_t* ullOfferIDA,
                                       std::function<int(int, int)> callback,
                                       bool bTrial) {
    return 0;
}
unsigned int StubStorage::GetAvailableDLCCount(int iPad) { return 0; }
StubStorage::EDLCStatus StubStorage::GetInstalledDLC(
    int iPad, std::function<int(int, int)> callback) {
    if (callback) {
        callback(0, iPad);
    }
    return EDLC_NoInstalledDLC;
}
XCONTENT_DATA& StubStorage::GetDLC(unsigned int dw) {
    return s_dummyContentData;
}
std::uint32_t StubStorage::MountInstalledDLC(
    int iPad, std::uint32_t dwDLC,
    std::function<int(int, std::uint32_t, std::uint32_t)> callback,
    const char* szMountDrive) {
    return 0;
}
unsigned int StubStorage::UnmountInstalledDLC(const char* szMountDrive) {
    return 0;
}
void StubStorage::GetMountedDLCFileList(const char* szMountDrive,
                                        std::vector<std::string>& fileList) {
    fileList.clear();
}
std::string StubStorage::GetMountedPath(std::string szMount) { return ""; }
StubStorage::ETMSStatus StubStorage::ReadTMSFile(
    int iQuadrant, eGlobalStorage eStorageFacility,
    StubStorage::eTMS_FileType eFileType, char* pwchFilename,
    std::uint8_t** ppBuffer, unsigned int* pBufferSize,
    std::function<int(char*, int, bool, int)> callback, int iAction) {
    return ETMSStatus_Fail;
}
bool StubStorage::WriteTMSFile(int iQuadrant, eGlobalStorage eStorageFacility,
                               char* pwchFilename, std::uint8_t* pBuffer,
                               unsigned int bufferSize) {
    return false;
}
bool StubStorage::DeleteTMSFile(int iQuadrant, eGlobalStorage eStorageFacility,
                                char* pwchFilename) {
    return false;
}
void StubStorage::StoreTMSPathName(char* pwchName) {}
StubStorage::ETMSStatus StubStorage::TMSPP_ReadFile(
    int iPad, StubStorage::eGlobalStorage eStorageFacility,
    StubStorage::eTMS_FILETYPEVAL eFileTypeVal, const char* szFilename,
    std::function<int(int, int, PTMSPP_FILEDATA, const char*)> callback,
    int iUserData) {
    return ETMSStatus_Fail;
}
unsigned int StubStorage::CRC(unsigned char* buf, int len) {
    unsigned int crc = 0xFFFFFFFF;
    for (int i = 0; i < len; i++) {
        crc ^= buf[i];
        for (int j = 0; j < 8; j++) {
            crc = (crc >> 1) ^ (0xEDB88320 & (-(crc & 1)));
        }
    }
    return ~crc;
}

int StubStorage::AddSubfile(int regionIndex) {
    for (size_t i = 0; i < m_subfiles.size(); i++) {
        if (m_subfiles[i].regionIndex == (unsigned int)regionIndex) {
            return (int)i;
        }
    }
    SubfileData sub;
    sub.regionIndex = (unsigned int)regionIndex;
    m_subfiles.push_back(std::move(sub));
    return (int)(m_subfiles.size() - 1);
}

unsigned int StubStorage::GetSubfileCount() {
    if (m_subfiles.empty() && !m_currentSaveTitle.empty()) {
        LoadFromDisk(m_currentSaveTitle);
    }
    return (unsigned int)m_subfiles.size();
}

void StubStorage::GetSubfileDetails(unsigned int i, int* regionIndex,
                                    void** data, unsigned int* size) {
    if (i < m_subfiles.size()) {
        if (regionIndex) *regionIndex = m_subfiles[i].regionIndex;
        if (size) *size = (unsigned int)m_subfiles[i].data.size();
        if (data) {
            if (m_subfiles[i].data.size() > 0) {
                void* copy = malloc(m_subfiles[i].data.size());
                memcpy(copy, m_subfiles[i].data.data(), m_subfiles[i].data.size());
                *data = copy;
            } else {
                *data = nullptr;
            }
        }
    } else {
        if (regionIndex) *regionIndex = 0;
        if (size) *size = 0;
        if (data) *data = nullptr;
    }
}

void StubStorage::ResetSubfiles() {
    m_subfiles.clear();
}

void StubStorage::UpdateSubfile(int index, void* data, unsigned int size) {
    if (index >= 0 && index < (int)m_subfiles.size()) {
        if (data && size > 0) {
            m_subfiles[index].data.assign((uint8_t*)data, (uint8_t*)data + size);
        } else {
            m_subfiles[index].data.clear();
        }
    }
}

void StubStorage::SaveSubfiles(std::function<int(const bool)> callback) {
    std::string safeName = sanitizeSaveName(m_currentSaveTitle);
    std::filesystem::path worldDir = getSavesRoot() / safeName;
    std::error_code ec;
    std::filesystem::create_directories(worldDir, ec);

    for (const auto& sub : m_subfiles) {
        if (!sub.data.empty()) {
            std::filesystem::path subPath = worldDir / ("subfile_" + std::to_string(sub.regionIndex) + ".bin");
            std::ofstream sf(subPath, std::ios::binary | std::ios::trunc);
            if (sf.is_open()) {
                sf.write(reinterpret_cast<const char*>(sub.data.data()), sub.data.size());
                sf.close();
            }
        }
    }
    if (callback) {
        callback(true);
    }
}
StubStorage::ESaveGameState StubStorage::GetSaveState() {
    return ESaveGame_Idle;
}
void StubStorage::ContinueIncompleteOperation() {}
