#pragma once

#include "plugin.h"
#include "discord-rpc.h"
#include <string>
#include <ctime>
#include <iostream>
#include <windows.h>

#ifdef GTASA
static const char* APPLICATION_ID = "698510493282992139";
#endif

#ifdef GTAVC
static const char* APPLICATION_ID = "929277207736643584";
#endif

#ifdef GTA3
static const char* APPLICATION_ID = "928498189236068393";
#endif

#ifdef GTAVC
#define GAME "VC"
#endif

#ifdef GTA3
#define GAME "III"
#endif

#ifdef GTASA
#define GAME "SA"
#endif

static std::ofstream LOGGER = std::ofstream("Discord_" GAME ".log");

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

	bool PlayerPointer();
	std::string PlayerTime();
	int PlayerMoneyValue();
	int PlayerCurrentWeapon();
	bool PlayerCondition();
};

const std::string weaponNames[] =
{
#ifdef GTASA
	{ "Fist" },
	{ "Brass Knuckles" },
	{ "Golf Club" },
	{ "Night Stick" },
	{ "Knife" },
	{ "Bat" },
	{ "Shovel" },
	{ "Pool Cue" },
	{ "Katana" },
	{ "Chainsaw" },
	{ "Purple Dildo" },
	{ "Dildo" },
	{ "Vibrator" },
	{ "Silver Vibrator" },
	{ "Flowers" },
	{ "Cane" },
	{ "Grenade" },
	{ "Teargas" },
	{ "Molotov" },
	{ " " }, // Unused
	{ " " }, // Unused
	{ " " }, // Unused
	{ "Colt 45" },
	{ "Silenced Pistol" },
	{ "Desert Eagle" },
	{ "Shotgun" },
	{ "Sawnoff-Shotgun" },
	{ "Combat Shotgun" },
	{ "Uzi" },
	{ "MP5" },
	{ "AK-47" },
	{ "M4" },
	{ "Tec-9" },
	{ "Country Rifle" },
	{ "Sniper Rifle" },
	{ "Rocket Launcher" },
	{ "Heat-Seeking RPG" },
	{ "Flamethrower" },
	{ "Minigun" },
	{ "Satchel Charges" },
	{ "Detonator" },
	{ "Spray Can" },
	{ "Fire Extinguisher" },
	{ "Camera" },
	{ "Night Vision" },
	{ "Thermal Goggles" },
	{ "Parachute" },
	{ "Fake Pistol" }
#endif

#ifdef GTAVC
	{ "Fist" },
	{ "Brass Knuckle"},
	{ "Screwdriver" },
	{ "Golf Club" },
	{ "Nightstick" },
	{ "Knife" },
	{ "Baseball" },
	{ "Hammer" },
	{ "Cleaver" },
	{ "Machete" },
	{ "Katana" },
	{ "Chainsaw" },
	{ "Grenade" },
	{ "Remote Grenade" },
	{ "Teargas" },
	{ "Molotov" },
	{ " " }, // Rocket
	{ "Colt 45" },
	{ "Python" },
	{ "Shotgun" },
	{ "Spas-12 Shotgun" },
	{ "Stubby Shotgun" },
	{ "Tec-9" },
	{ "Uzi" },
	{ "Silenced Ingram" },
	{ "MP5" },
	{ "M4" },
	{ "Kruger" },
	{ "Sniper-Rifle" },
	{ "Laser Scope" },
	{ "Rocket Launcher" },
	{ "Flame Thrower" },
	{ "M60" },
	{ "Minigun" },
	{ "Detonator" },
	{ " " }, // Helicannon
	{ "Camera" }
#endif

#ifdef GTA3
 { " " } // unused because we cannot find the weapon memory addresses for this one
#endif

};

#ifdef GTASA
const std::string weaponIcons[] =
{
	{ "fist" },
	{ "brassknuckleicon" },
	{ "golfclubicon" },
	{ "nitestickicon" },
	{ "knifecuricon" },
	{ "baticon" },
	{ "shovelicon" },
	{ "poolcueicon" },
	{ "katanaicon" },
	{ "chnsawicon" },
	{ "dildo1icon" },
	{ "dildo2icon" },
	{ "vibe1icon" },
	{ "vibe2icon" },
	{ "flowericon" },
	{ "caneicon" },
	{ "grenadeicon" },
	{ "teargasicon" },
	{ "molotovicon" },
	{ " " },
	{ " " },
	{ " " },
	{ "colt45icon" },
	{ "silencedicon" },
	{ "desert_eagleicon" },
	{ "chromegunicon" },
	{ "sawnofficon" },
	{ "shotgspaicon" },
	{ "micro_uziicon" },
	{ "mp5lngicon" },
	{ "ak47icon" },
	{ "m4icon" },
	{ "tec9icon" },
	{ "cuntgunicon" },
	{ "snipericon" },
	{ "rocketlaicon" },
	{ "heatseekicon" },
	{ "flameicon" },
	{ "minigunicon" },
	{ "satchelicon" },
	{ "bombicon" },
	{ "spraycanicon" },
	{ "fire_exicon" },
	{ "cameraicon" },
	{ "gogglesicon" },
	{ "gogglesicon" },
	{ "paraicon" },
};
#endif

extern Game* rpc;
#ifdef GTASA
extern HMODULE module;
#endif