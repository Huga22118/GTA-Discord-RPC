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

#ifdef GTASA
void RPC()
{
	while (*(DWORD*)0xC8D4C0 != 9)
		Sleep(350);

	HMODULE module = NULL;
	rpc = new Game();

	if (GetModuleHandleA("SAMP.dll") || GetModuleHandleA("SAMP.asi"))
	{
		FreeLibraryAndExitThread(module, 0);
		MessageBox(RsGlobal.ps->window, "SA-MP are not compatible with this mod.", "Discord-RPC for GTA SA 1.0 Hoodlum", MB_ICONERROR);
	}
	else
	{
		std::string details, state, largeImageText, smallImageText;

		DiscordRichPresence drp;

		drp = { 0 };
		drp.startTimestamp = time(0);
		Discord_Initialize(APPLICATION_ID, 0, 0, 0);

		while (1)
		{
			if (rpc->PlayerPointer())
			{

				details = "Money: $" + std::to_string(rpc->PlayerMoneyValue());
				state = "Time: " + rpc->PlayerTime();
				smallImageText = weaponNames[rpc->PlayerCurrentWeapon()];
				largeImageText = "GTA SA Discord RPC";


				drp.largeImageKey = "game_icon";

				drp.smallImageText = smallImageText.c_str();
				drp.largeImageText = largeImageText.c_str();
				drp.smallImageKey = weaponIcons[rpc->PlayerCurrentWeapon()].c_str();;
				drp.details = details.c_str();
				drp.state = state.c_str();

				Discord_UpdatePresence(&drp);
				Sleep(100);
			}
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

			largeImageText = "GTA VC Discord RPC";

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

	while (1)
	{
		if (rpc->PlayerPointer())
		{

			details = "In-Game";
			state = "Money: $" + std::to_string(rpc->PlayerMoneyValue());

			largeImageText = "GTA III Discord RPC";

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

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	switch (reason)
	{

	case DLL_PROCESS_ATTACH:
	{

		if (GetGameVersion() == BY_GAME(GAME_10US_HOODLUM, GAME_10EN, GAME_10EN))
		{

			CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)&RPC, nullptr, NULL, nullptr);

		}
		else
		{
			MessageBox(HWND_DESKTOP, "Unknown GTA Game Version. GTA " BY_GAME("SA 1.0 Hoodlum", "VC 1.0 English", "III 1.0 English") " is required in order to use this mod, other versions are not compatible", "Discord-RPC for GTA " BY_GAME("SA 1.0 Hoodlum", "VC 1.0 English", "III 1.0 English"), MB_ICONERROR);
		}
		break;
	}

	case DLL_PROCESS_DETACH:
	{

		Discord_Shutdown();

		break;

	}
	}

	return TRUE;
}