#pragma once

#include <cstdint>
#include <string>

#include "platform/PlatformTypes.h"

// XUI handle type (used by App_structs.h for SceneStackPair)
typedef struct _XUIOBJ* HXUIOBJ;

bool IsEqualXUID(PlayerUID a, PlayerUID b);

// Gamepad virtual key constants
#define VK_PAD_A 0x5800
#define VK_PAD_B 0x5801
#define VK_PAD_X 0x5802
#define VK_PAD_Y 0x5803
#define VK_PAD_RSHOULDER 0x5804
#define VK_PAD_LSHOULDER 0x5805
#define VK_PAD_LTRIGGER 0x5806
#define VK_PAD_RTRIGGER 0x5807

#define VK_PAD_DPAD_UP 0x5810
#define VK_PAD_DPAD_DOWN 0x5811
#define VK_PAD_DPAD_LEFT 0x5812
#define VK_PAD_DPAD_RIGHT 0x5813
#define VK_PAD_START 0x5814
#define VK_PAD_BACK 0x5815
#define VK_PAD_LTHUMB_PRESS 0x5816
#define VK_PAD_RTHUMB_PRESS 0x5817

#define VK_PAD_LTHUMB_UP 0x5820
#define VK_PAD_LTHUMB_DOWN 0x5821
#define VK_PAD_LTHUMB_RIGHT 0x5822
#define VK_PAD_LTHUMB_LEFT 0x5823
#define VK_PAD_LTHUMB_UPLEFT 0x5824
#define VK_PAD_LTHUMB_UPRIGHT 0x5825
#define VK_PAD_LTHUMB_DOWNRIGHT 0x5826
#define VK_PAD_LTHUMB_DOWNLEFT 0x5827

#define VK_PAD_RTHUMB_UP 0x5830
#define VK_PAD_RTHUMB_DOWN 0x5831
#define VK_PAD_RTHUMB_RIGHT 0x5832
#define VK_PAD_RTHUMB_LEFT 0x5833
#define VK_PAD_RTHUMB_UPLEFT 0x5834
#define VK_PAD_RTHUMB_UPRIGHT 0x5835
#define VK_PAD_RTHUMB_DOWNRIGHT 0x5836
#define VK_PAD_RTHUMB_DOWNLEFT 0x5837

// XUI string table
class CXuiStringTable {
public:
    const char* Lookup(const char* szId);
    const char* Lookup(uint32_t nIndex);
    void Clear();
    int32_t Load(const char* szId);
};

// User sign-in
struct XUSER_SIGNIN_INFO {
    PlayerUID xuid;
    uint32_t dwGuestNumber;
};
using PXUSER_SIGNIN_INFO = XUSER_SIGNIN_INFO*;

#define XUSER_GET_SIGNIN_INFO_ONLINE_XUID_ONLY 0x00000001
#define XUSER_GET_SIGNIN_INFO_OFFLINE_XUID_ONLY 0x00000002

uint32_t XUserGetSigninInfo(uint32_t dwUserIndex, uint32_t dwFlags,
                            PXUSER_SIGNIN_INFO pSigninInfo);

// Locale/language constants
inline constexpr int XC_LANGUAGE_ENGLISH = 0x01;
inline constexpr int XC_LANGUAGE_JAPANESE = 0x02;
inline constexpr int XC_LANGUAGE_GERMAN = 0x03;
inline constexpr int XC_LANGUAGE_FRENCH = 0x04;
inline constexpr int XC_LANGUAGE_SPANISH = 0x05;
inline constexpr int XC_LANGUAGE_ITALIAN = 0x06;
inline constexpr int XC_LANGUAGE_KOREAN = 0x07;
inline constexpr int XC_LANGUAGE_TCHINESE = 0x08;
inline constexpr int XC_LANGUAGE_PORTUGUESE = 0x09;
inline constexpr int XC_LANGUAGE_POLISH = 0x0B;
inline constexpr int XC_LANGUAGE_RUSSIAN = 0x0C;
inline constexpr int XC_LANGUAGE_SWEDISH = 0x0D;
inline constexpr int XC_LANGUAGE_TURKISH = 0x0E;
inline constexpr int XC_LANGUAGE_BNORWEGIAN = 0x0F;
inline constexpr int XC_LANGUAGE_DUTCH = 0x10;
inline constexpr int XC_LANGUAGE_SCHINESE = 0x11;
inline constexpr int XC_LANGUAGE_LATINAMERICANSPANISH = 0xF0;
inline constexpr int XC_LANGUAGE_FINISH = 0xF1;
inline constexpr int XC_LANGUAGE_GREEK = 0xF2;
inline constexpr int XC_LANGUAGE_DANISH = 0xF3;
inline constexpr int XC_LANGUAGE_CZECH = 0xF4;
inline constexpr int XC_LANGUAGE_SLOVAK = 0xF5;

inline constexpr int XC_LOCALE_AUSTRALIA = 1;
inline constexpr int XC_LOCALE_AUSTRIA = 2;
inline constexpr int XC_LOCALE_BELGIUM = 3;
inline constexpr int XC_LOCALE_BRAZIL = 4;
inline constexpr int XC_LOCALE_CANADA = 5;
inline constexpr int XC_LOCALE_CHILE = 6;
inline constexpr int XC_LOCALE_CHINA = 7;
inline constexpr int XC_LOCALE_COLOMBIA = 8;
inline constexpr int XC_LOCALE_CZECH_REPUBLIC = 9;
inline constexpr int XC_LOCALE_DENMARK = 10;
inline constexpr int XC_LOCALE_FINLAND = 11;
inline constexpr int XC_LOCALE_FRANCE = 12;
inline constexpr int XC_LOCALE_GERMANY = 13;
inline constexpr int XC_LOCALE_GREECE = 14;
inline constexpr int XC_LOCALE_HONG_KONG = 15;
inline constexpr int XC_LOCALE_HUNGARY = 16;
inline constexpr int XC_LOCALE_INDIA = 17;
inline constexpr int XC_LOCALE_IRELAND = 18;
inline constexpr int XC_LOCALE_ITALY = 19;
inline constexpr int XC_LOCALE_JAPAN = 20;
inline constexpr int XC_LOCALE_KOREA = 21;
inline constexpr int XC_LOCALE_MEXICO = 22;
inline constexpr int XC_LOCALE_NETHERLANDS = 23;
inline constexpr int XC_LOCALE_NEW_ZEALAND = 24;
inline constexpr int XC_LOCALE_NORWAY = 25;
inline constexpr int XC_LOCALE_POLAND = 26;
inline constexpr int XC_LOCALE_PORTUGAL = 27;
inline constexpr int XC_LOCALE_SINGAPORE = 28;
inline constexpr int XC_LOCALE_SLOVAK_REPUBLIC = 29;
inline constexpr int XC_LOCALE_SOUTH_AFRICA = 30;
inline constexpr int XC_LOCALE_SPAIN = 31;
inline constexpr int XC_LOCALE_SWEDEN = 32;
inline constexpr int XC_LOCALE_SWITZERLAND = 33;
inline constexpr int XC_LOCALE_TAIWAN = 34;
inline constexpr int XC_LOCALE_GREAT_BRITAIN = 35;
inline constexpr int XC_LOCALE_UNITED_STATES = 36;
inline constexpr int XC_LOCALE_RUSSIAN_FEDERATION = 37;
inline constexpr int XC_LOCALE_WORLD_WIDE = 38;
inline constexpr int XC_LOCALE_TURKEY = 39;
inline constexpr int XC_LOCALE_ARGENTINA = 40;
inline constexpr int XC_LOCALE_SAUDI_ARABIA = 41;
inline constexpr int XC_LOCALE_ISRAEL = 42;
inline constexpr int XC_LOCALE_UNITED_ARAB_EMIRATES = 43;
inline constexpr int XC_LOCALE_LATIN_AMERICA = 240;

uint32_t XGetLanguage();
uint32_t XGetLocale();
uint32_t XEnableGuestSignin(bool fEnable);

// System notification constants (used by Game.cpp notification handler)
inline constexpr int XN_SYS_SIGNINCHANGED = 0;
inline constexpr int XN_SYS_INPUTDEVICESCHANGED = 1;
inline constexpr int XN_LIVE_CONTENT_INSTALLED = 2;
inline constexpr int XN_SYS_STORAGEDEVICESCHANGED = 3;

inline constexpr int XZP_ICON_SHANK_01 = 1;
inline constexpr int XZP_ICON_SHANK_02 = 2;
inline constexpr int XZP_ICON_SHANK_03 = 3;
