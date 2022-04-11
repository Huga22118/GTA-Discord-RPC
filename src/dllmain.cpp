#include "dllmain.h"

using namespace plugin;

Game* rpc;

int DCStatus = 1;
bool ShowDetectedIni = true;
bool ShowLOG = true;

/*
    Code Taken from MixSets Repo!!
*/
bool Game::IniBool(CIniReader ini, std::string section, std::string key)
{	
	bool b = ini.ReadInteger(section, key, 0) == 1;
	if (b == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Game::Init()
{
	CIniReader ini("Discord_" GAME ".ini");
	DCStatus = IniBool(ini, "Discord", "DiscordStatus");
	ShowDetectedIni = IniBool(ini, "Discord", "ShowDetectedINIMsg");
	ShowLOG = IniBool(ini, "Discord", "EnableLogger");

	if (ini.data.size() <= 0)
	{
		Error("Discord_%s.ini not found!", GAME);
		IfIniFailed = true;
		return;
	}

	if (ShowDetectedIni)
	{
		Message("Discord_%s.ini Detected!", GAME);
	}

	if (ShowLOG)
	{
		static char   sBufferz[1024];
		sprintf(sBufferz, "GTA Trilogy Discord RPC\nv4.1\n");
		LOGGER << sBufferz << std::endl;
	}

	if (ShowLOG)
	{
		static char   sBufferz[1024];
		sprintf(sBufferz, "%s Detected.\n", GetGameVersionName());
		LOGGER << sBufferz << std::endl;
	}

	if (ShowLOG)
	{
		Logger("Log started");
	}

	rpc->Update();
}

void Game::Update()
{
	if (ShowLOG)
	{
		Logger("Trying to start void Game::Update().");
	}

	Events::initRwEvent += []
	{
		CIniReader ini;

		rpc = new Game();
		std::string details, state, largeImageText, smallImageText;

		DiscordRichPresence drp;

		drp = { 0 };
		drp.startTimestamp = time(0);
		Discord_Initialize(APPLICATION_ID, 0, 0, 0);

		if (ShowLOG)
		{
			Logger("void Game::Update() Started!");
		}

		if (DCStatus)
		{
#ifdef GTA3
			if (GetGameVersion() == GAME_10EN || GetGameVersion() == GAME_11EN || GetGameVersion() == GAME_STEAM)
			{
				details = "Game: III";
			}
#elif GTASA
			if (GetGameVersion() == GAME_10US_HOODLUM || GetGameVersion() == GAME_10US_COMPACT ||
				GetGameVersion() == GAME_10EU || GetGameVersion() == GAME_11US || 
				GetGameVersion() == GAME_11EU || GetGameVersion() == GAME_STEAM ||
				GetGameVersion() == GAME_STEAM_LV)
			{
				details = "Game: San Andreas";
			}
#elif GTAVC
			if (GetGameVersion() == GAME_10EN || GetGameVersion() == GAME_11EN || GetGameVersion() == GAME_STEAM)
			{
				details = "Game: Vice City";
			}
#endif
#ifdef GTA3
			if (GetGameVersion() == GAME_10EN || GetGameVersion() == GAME_11EN || GetGameVersion() == GAME_STEAM)
			{
				static char  sBufferz[1024];
				sprintf(sBufferz, "Version: %s", GetGameVersionNames());
				state = sBufferz;
			}
#elif GTASA
			if (GetGameVersion() == GAME_10US_HOODLUM || GetGameVersion() == GAME_10US_COMPACT ||
				GetGameVersion() == GAME_10EU || GetGameVersion() == GAME_11US ||
				GetGameVersion() == GAME_11EU || GetGameVersion() == GAME_STEAM ||
				GetGameVersion() == GAME_STEAM_LV)
			{
				static char  sBufferz[1024];
				sprintf(sBufferz, "Version: %s", GetGameVersionNames());
				state = sBufferz;
			}
#elif GTAVC
			if (GetGameVersion() == GAME_10EN || GetGameVersion() == GAME_11EN || GetGameVersion() == GAME_STEAM)
			{
				static char  sBufferz[1024];
				sprintf(sBufferz, "Version: %s", GetGameVersionNames());
				state = sBufferz;
		}
#endif
			drp.details = details.c_str();
			drp.state = state.c_str();
		}

		largeImageText = "GTA Trilogy Discord RPC Plugin";

#ifdef GTA3
		drp.largeImageKey = "iii";
#elif GTASA
		drp.largeImageKey = "sa";
#elif GTAVC
		drp.largeImageKey = "vc";
#endif

		drp.smallImageText = smallImageText.c_str();
		drp.largeImageText = largeImageText.c_str();

		Discord_UpdatePresence(&drp);
		Sleep(100);
	};
}

BOOL APIENTRY DllMain(HINSTANCE hDllHandle, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{

	case DLL_PROCESS_ATTACH:
	{
#ifdef GTASA
		if (GetModuleHandleA("SAMP.dll") || GetModuleHandleA("SAMP.asi"))
		{
			if (GetGameVersion() != GAME_10US_HOODLUM || GetGameVersion() != GAME_10US_COMPACT ||
				GetGameVersion() != GAME_10EU || GetGameVersion() != GAME_11US ||
				GetGameVersion() != GAME_11EU || GetGameVersion() != GAME_STEAM ||
				GetGameVersion() != GAME_STEAM_LV)
			{
				Error("SA-MP Detected and this plugin are incompatible with this version \n- %s", GetGameVersionName());
				IfSampExist = true;
			}
			else
			{
				Error("SA-MP Detected!");
				IfSampExist = true;
			}
		}
		else
		{
			if (GetGameVersion() == GAME_10US_HOODLUM || GetGameVersion() == GAME_10US_COMPACT ||
				GetGameVersion() == GAME_10EU || GetGameVersion() == GAME_11US ||
				GetGameVersion() == GAME_11EU || GetGameVersion() == GAME_STEAM ||
				GetGameVersion() == GAME_STEAM_LV)
			{
				rpc->Init();
				IfSampExist = false;
			}
			else
			{
				Error("This game version is not supported by %s plugin.\nThis plugin supports these game versions:\n- %s \n- %s \n- %s \n- %s \n- %s \n- %s \n- %s",
					plugin::paths::GetPluginFileNameA(), GetGameVersionName(GAME_10US_HOODLUM), GetGameVersionName(GAME_10US_COMPACT),
					GetGameVersionName(GAME_10EU), GetGameVersionName(GAME_11US), GetGameVersionName(GAME_11EU), GetGameVersionName(GAME_STEAM),
					GetGameVersionName(GAME_STEAM_LV));
				IfSampExist = false;
			}
		}
#else
		if (GetGameVersion() == GAME_10EN || GetGameVersion() == GAME_11EN || GetGameVersion() == GAME_STEAM)
		{
			rpc->Init();
		}
		else
		{
			Error("This game version is not supported by %s plugin.\nThis plugin supports these game versions:\n- %s \n- %s \n- %s",
				plugin::paths::GetPluginFileNameA(), GetGameVersionName(GAME_10EN), GetGameVersionName(GAME_11EN),
				GetGameVersionName(GAME_STEAM));
		}
#endif
			break;
	}

	case DLL_PROCESS_DETACH:
	{
		if (ShowLOG)
		{
			Logger("%s has been shutdown, Detaching.", GetGameVersionName());
		}

		Discord_Shutdown();

		Sleep(350);

		break;

	}
	}

	return TRUE;
}  