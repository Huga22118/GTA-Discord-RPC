#include "dllmain.h"

using namespace plugin;

Game* rpc;

bool Game::PlayerPointer()
{
#ifdef GTASA
	return *reinterpret_cast<int*>(0xB6F5F0);
#endif

#ifdef GTAVC
	return *reinterpret_cast<int*>(0x94AD28);
#endif

#ifdef GTA3
	return *reinterpret_cast<int*>(0x6FB1C8);
#endif
}

int Game::PlayerMoneyValue()
{
#ifdef GTASA
	return *reinterpret_cast<int*>(0xB7CE50);
#endif

#ifdef GTA3
	return *reinterpret_cast<int*>(0x94139C);
#endif
}

std::string Game::PlayerTime()
{
	BYTE minutes = *reinterpret_cast<BYTE*>(0xB70152);
	std::string minstr;
	if (minutes < 10) minstr = "0" + std::to_string(minutes);
	else minstr = std::to_string(minutes);
	return std::to_string(*reinterpret_cast<BYTE*>(0xB70153)) + ":" + minstr;
}

int Game::PlayerCurrentWeapon()
{
#ifdef GTASA
	return *reinterpret_cast<int*>(0xBAA410);
#endif

#ifdef GTAVC
	return *reinterpret_cast<unsigned int*>(0x978DA8);
#endif
}

bool Game::PlayerCondition()
{
	return *reinterpret_cast<int*>(*reinterpret_cast<int*>(0x94AD28) + 0x3AC);
}

float Game::SAPlayerHealth()
{
	try 
	{
		return *reinterpret_cast<float*>(*reinterpret_cast<int*>(0xB6F5F0) + 0x540);
	}
	catch (...)
	{
		return 0.0;
	}
}

float Game::SAPlayerArmour()
{
	try
	{
		return *reinterpret_cast<float*>(*reinterpret_cast<int*>(0xB6F5F0) + 0x548);
	}
	catch (...)
	{
		return 0.0;
	}
}

#ifdef GTASA
void RPC()
{
	int DCStatus = 1;
	bool ShowDetectedIni = true;

	CIniReader ini("Discord_" GAME ".ini");

	if (ini.data.size() <= 0)
	{
		Logger("Error: Discord_%s.ini not found!", GAME);
		Error("Discord_%s.ini not found!", GAME);
		IfIniFailed = true;
	}
	else
	{
		Logger("Success: Discord_%s.ini Loaded", GAME);
		IfIniFailed = false;
	}

	DCStatus = ini.ReadInteger("Discord", "DiscordStatus", 1);
	ShowDetectedIni = ini.ReadInteger("Discord", "ShowDetectedINIMsg", 0);

	if (ShowDetectedIni)
	{
		Message("Discord_%s.ini Detected!", GAME);
	}

	while (*(DWORD*)0xC8D4C0 != 9)
		Sleep(350);

	    HMODULE module = NULL;
	    rpc = new Game();
		std::string details, state, largeImageText, smallImageText;

		DiscordRichPresence drp;

		drp = { 0 };
		drp.startTimestamp = time(0);
		Discord_Initialize(APPLICATION_ID, 0, 0, 0);

		Logger("Success: Discord Module initialized.");

		while (1)
		{
			if (rpc->PlayerPointer())
			{

				if (DCStatus == 1)
				{
					details = "Money: $" + std::to_string(rpc->PlayerMoneyValue());
					state = "Time: " + rpc->PlayerTime();

					drp.details = details.c_str();
					drp.state = state.c_str();
				}
				else if (DCStatus == 2)
				{
					char health[64];
					sprintf_s(health, "Health: %.2f%%", rpc->SAPlayerHealth());
					details = health;

					char armour[64];
					sprintf_s(armour, "Armour: %.2f%%", rpc->SAPlayerArmour());
					state = armour;

					drp.details = details.c_str();
					drp.state = state.c_str();
				}

				smallImageText = weaponNames[rpc->PlayerCurrentWeapon()];
				largeImageText = GetGameVersionName();


				drp.largeImageKey = "game_icon";

				drp.smallImageText = smallImageText.c_str();
				drp.largeImageText = largeImageText.c_str();
				drp.smallImageKey = weaponIcons[rpc->PlayerCurrentWeapon()].c_str();;

				Discord_UpdatePresence(&drp);
				Sleep(100);
			}
	}
}
#endif

#ifdef GTAVC
void RPC()
{
    while (*(DWORD*)0x9B5F08 != 9)
				Sleep(350);

	rpc = new Game();
	std::string details, state, largeImageText;

	DiscordRichPresence drp;

	drp = { 0 };
	drp.startTimestamp = time(0);
	Discord_Initialize(APPLICATION_ID, 0, 0, 0);

	Logger("Success: Discord Module initialized.");

	while (1)
	{
		if (rpc->PlayerPointer())
		{
			if (rpc->PlayerCondition())
			{
				details = "On Vehicle";
			}
			else
			{
				details = "On Foot";
	        }

			state = "Weapon: " + weaponNames[rpc->PlayerCurrentWeapon()];

			largeImageText = GetGameVersionName();

			drp.largeImageKey = "game_icon";

			drp.largeImageText = largeImageText.c_str();
			drp.details = details.c_str();
			drp.state = state.c_str();

			Discord_UpdatePresence(&drp);
			Sleep(100);
        }
	}
}
#endif

#ifdef GTA3
void RPC()
{
	while (*(DWORD*)0x8F5838 != 9)
    Sleep(350);

	rpc = new Game();
	std::string details, state, largeImageText;

	DiscordRichPresence drp;

	drp = { 0 };
	drp.startTimestamp = time(0);
	Discord_Initialize(APPLICATION_ID, 0, 0, 0);

	Logger("Success: Discord Module initialized.");

	while (1)
	{
		if (rpc->PlayerPointer())
		{

			details = "In-Game";
			state = "Money: $" + std::to_string(rpc->PlayerMoneyValue());

			largeImageText = GetGameVersionName();

			drp.largeImageKey = "icon";
			drp.largeImageText = largeImageText.c_str();
			drp.details = details.c_str();
			drp.state = state.c_str();

			Discord_UpdatePresence(&drp);
			Sleep(100);
		}
	}
}
#endif

void Attach()
{
#ifdef GTASA
	if (GetModuleHandleA("SAMP.dll") || GetModuleHandleA("SAMP.asi"))
	{
		Error("SA-MP Detected!");
		Logger("Error: SA-MP are not compatible with this mod.");
		IfSampExist = true;
	}
	else
	{
		CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)&RPC, nullptr, NULL, nullptr);
		IfSampExist = false;
	}
#else
	    CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)&RPC, nullptr, NULL, nullptr);
#endif

	
}

void Detach()
{
	Events::shutdownRwEvent += []
	{
		Discord_Shutdown();
	};
}

BOOL APIENTRY DllMain(HINSTANCE hDllHandle, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{

	case DLL_PROCESS_ATTACH:
	{

#ifdef GTASA
		if (GetGameVersion() == GAME_10US_HOODLUM || GetGameVersion() == GAME_10US_COMPACT)
#else
		if (GetGameVersion() == GAMELIST(NULL, GAME_10EN, GAME_10EN))
#endif

		{
			CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)&Attach, nullptr, NULL, nullptr);
			Logger("Success: %s is compatible with this mod, now waiting for Discord Module initialized.", GetGameVersionName());
		}
		else
		{
#ifdef GTASA	
			Error("This game version is not supported by %s plugin.\nThis plugin supports these game versions:\n- %s \n- %s",
				plugin::paths::GetPluginFileNameA(), GetGameVersionName(GAME_10US_HOODLUM), GetGameVersionName(GAME_10US_COMPACT));
			Logger("Error: %s is not compatible with this mod, these are supported version:\n%s & %s", GetGameVersionName(), GetGameVersionName(GAME_10US_HOODLUM), GetGameVersionName(GAME_10US_COMPACT));
#else
			Error("This game version is not supported by %s plugin.\nThis plugin supports these game versions:\n- %s",
				plugin::paths::GetPluginFileNameA(), GetGameVersionName(GAME_10EN));
			Logger("Error: %s is not compatible with this mod, these are supported version:\n%s", GetGameVersionName(), GetGameVersionName(GAME_10EN));
#endif
		}
		break;
	}

	case DLL_PROCESS_DETACH:
	{
		Logger("%s has been shutdown, Detaching.", GetGameVersionName());
		Sleep(350);

		CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)&Detach, nullptr, NULL, nullptr);

		break;

	}
	}

	return TRUE;
}  