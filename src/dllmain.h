#pragma once

#include "plugin.h"
#include "discord-rpc.h"
#include "ini/IniReader.h"
#include <string>
#include <ctime>
#include <iostream>
#include <windows.h>
#include <GameVersion.h>

static const char* APPLICATION_ID = "946628098693144626";

#ifdef GTAVC
#define GAME "VC"
#define GAMELIST(sa_compacthoodlum, vc, iii) vc
#endif

#ifdef GTA3
#define GAME "III"
#define GAMELIST(sa_compacthoodlum, vc, iii) iii
#endif

#ifdef GTASA
#define GAME "SA"
#define GAMELIST(sa_compacthoodlum, vc, iii) sa_compacthoodlum
#endif

static std::ofstream LOGGER = std::ofstream("Discord_" GAME ".log");
static inline bool IfIniFailed;
static inline bool IfSampExist;

extern int DCStatus;
extern bool ShowDetectedIni;
extern bool ShowLOG;

void Logger(const char* format, ...)
{
	SYSTEMTIME timemonthday;
	static char   sBufferz[1024];

	GetLocalTime(&timemonthday);
	sprintf(sBufferz, "%02d/%02d/%04d %02d:%02d:%02d.%03d ", timemonthday.wDay, timemonthday.wMonth, timemonthday.wYear, timemonthday.wHour, timemonthday.wMinute, timemonthday.wSecond, timemonthday.wMilliseconds);
	va_list arg;
	va_start(arg, format);
	vsprintf(sBufferz + strlen(sBufferz), format, arg);
	va_end(arg);
	LOGGER << sBufferz << std::endl;
	OutputDebugString(sBufferz);
	OutputDebugString("\n");
}

class Game
{
public:

	void Update();
	void Init();
	static bool IniBool(CIniReader ini, std::string section, std::string key);

};

inline char const* GetGameVersionNames(int gameVersionId) {
    switch (gameVersionId) {
#ifdef GTASA
    case GAME_10US_COMPACT:
        return "1.0 US Compact";
    case GAME_10US_HOODLUM:
        return "1.0 US Hoodlum";
    case GAME_10EU:
        return "1.0 EU";
    case GAME_11US:
        return "1.01 US";
    case GAME_11EU:
        return "1.01 EU";
    case GAME_STEAM:
        return "Steam";
    case GAME_STEAM_LV:
        return "Steam LV";
#endif
#ifdef GTAVC
    case GAME_10EN:
        return "1.0 EN";
    case GAME_11EN:
        return "1.1 EN";
    case GAME_STEAM:
        return "Steam";
#endif
#ifdef GTA3
    case GAME_10EN:
        return "1.0 EN";
    case GAME_11EN:
        return "1.1 EN";
    case GAME_STEAM:
        return "Steam";
#endif
    }
    return "Unknown";
}

inline char const* GetGameVersionNames() 
{
    return GetGameVersionNames(plugin::GetGameVersion());
}

extern Game* rpc;