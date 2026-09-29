#pragma once

#include <cstdint>
#include <cstring>
#include <vector>

#include "app/common/App_structs.h"
#include "platform/XboxStubs.h"

class BannedListManager {
public:
    BannedListManager();

    void invalidate(int iPad);
    void addLevel(int iPad, PlayerUID xuid, char* pszLevelName,
                  bool bWriteToTMS);
    bool isInList(int iPad, PlayerUID xuid, char* pszLevelName);
    void removeLevel(int iPad, PlayerUID xuid, char* pszLevelName);

    void setUniqueMapName(char* pszUniqueMapName);
    char* getUniqueMapName();

    void setBanListCheck(int iPad, bool bVal) { m_BanListCheck[iPad] = bVal; }
    bool getBanListCheck(int iPad) const { return m_BanListCheck[iPad]; }

    bool getBanListRead(int iPad) const { return m_bRead_BannedListA[iPad]; }
    void setBanListRead(int iPad, bool bVal) {
        m_bRead_BannedListA[iPad] = bVal;
    }

    void clearBanList(int iPad) {
        BannedListA[iPad].pBannedList = nullptr;
        BannedListA[iPad].byteCount = 0;
    }

    BANNEDLIST BannedListA[XUSER_MAX_COUNT];

    std::uint8_t* m_pBannedListFileBuffer;
    unsigned int m_dwBannedListFileSize;

private:
    VBANNEDLIST* m_vBannedListA[XUSER_MAX_COUNT];
    bool m_bRead_BannedListA[XUSER_MAX_COUNT];
    char m_pszUniqueMapName[14];
    bool m_BanListCheck[XUSER_MAX_COUNT];
};
