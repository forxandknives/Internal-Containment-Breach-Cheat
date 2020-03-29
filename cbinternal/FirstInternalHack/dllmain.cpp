// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <iostream>
#include "mem.h"

//https://guidedhacking.com/threads/how-to-hack-any-game-first-internal-hack-dll-tutorial.12142/

void setCursorPosition(int x, int y);
void ShowConsoleCursor(bool showFlag);

DWORD WINAPI HackThread(HMODULE hModule)
{
	//Create Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
	ShowConsoleCursor(false);

	uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"SCP - Containment Breach.exe");

	//calling it with NULL also gives you the address of the .exe module
	moduleBase = (uintptr_t)GetModuleHandle(NULL);

	bool bGodmode = false, bNoclip = false, bSprint = false, bBlink = false, bNotarget = false, bDebughud = false, bCamerafog = false;


	while (true)
	{
		std::cout << "Press 1 for Godmode\nPress 2 for Noclip\nPress 3 for Unlimited Sprint\nPress 4 for Unlimited Blink\nPress 5 for Notarget\nPress 6 for Debughud\nPress 7 for No Camerafog\nPress end to uninject\n";



		if (GetAsyncKeyState(VK_END) & 1)
		{
			break;
		}

		if (GetAsyncKeyState(0x31) & 1)
			bGodmode = !bGodmode;

		if (GetAsyncKeyState(0x32) & 1)
		{
			bNoclip = !bNoclip;
		}

		if (GetAsyncKeyState(0x33) & 1)
		{
			bSprint = !bSprint;
		}

		if (GetAsyncKeyState(0x34) & 1)
		{
			bBlink = !bBlink;
		}

		if (GetAsyncKeyState(0x35) & 1)
		{
			bNotarget = !bNotarget;
		}

		if (GetAsyncKeyState(0x36) & 1)
		{
			bDebughud = !bDebughud;
		}

		if (GetAsyncKeyState(0x37) & 1)
		{
			bCamerafog = !bCamerafog;
		}

		//need to use uintptr_t for pointer arithmetic later
		uintptr_t* localPlayerPtr = (uintptr_t*)(moduleBase + 0xf5400);

		//continuous writes / freeze

		if (localPlayerPtr)
		{
			if (bGodmode)
			{
				*(int*)mem::FindDMAAddy(moduleBase + 0xf5400, { 0x98, 0x5b0 }) = 1;
				std::cout << ' ' << std::endl;
				std::cout << "1.Godmode enabled.   \n";
			}
			else
			{
				*(int*)mem::FindDMAAddy(moduleBase + 0xf5400, { 0x98, 0x5b0 }) = 0;
				std::cout << ' ' << std::endl;
				std::cout << "1.Godmode disabled.   \n";
			}


			if (bNoclip)
			{
				*(int*)mem::FindDMAAddy(moduleBase + 0xf5400, { 0x98, 0x5b4 }) = 1;
				std::cout << "2.Noclip enabled.   \n";
			}
			else
			{
				*(int*)mem::FindDMAAddy(moduleBase + 0xf5400, { 0x98, 0x5b4 }) = 0;
				std::cout << "2.Noclip disabled.   \n";
			}

			if (bSprint)
			{
				*(float*)mem::FindDMAAddy(moduleBase + 0xf5400, { 0x98, 0x394 }) = 99;
				std::cout << "3.Infinite Sprint enabled.     \n";
			}
			else
				std::cout << "3.Infinite Sprint disabled.    \n";


			if (bBlink)
			{
				*(float*)mem::FindDMAAddy(moduleBase + 0xf5400, { 0x98, 0x380 }) = 550;
				std::cout << "4.Infinite Blink enabled.     \n";
			}
			else
				std::cout << "4.Infinite Blink disabled.     \n";


			if (bNotarget)
			{
				*(int*)mem::FindDMAAddy(moduleBase + 0xf5400, { 0x98, 0xfd4 }) = 1;
				std::cout << "5.Notarget enabled.    \n";
			}
			else
			{
				*(int*)mem::FindDMAAddy(moduleBase + 0xf5400, { 0x98, 0xfd4 }) = 0;
				std::cout << "5.Notarget disabled.     \n";
			}


			if (bDebughud)
			{
				*(int*)mem::FindDMAAddy(moduleBase + 0xf5400, { 0x98, 0xad0 }) = 1;
				std::cout << "6.Debughud enabled.    \n";
			}
			else
			{
				*(int*)mem::FindDMAAddy(moduleBase + 0xf5400, { 0x98, 0xad0 }) = 0;
				std::cout << "6.Debughud disabled.     \n";
			}

			if (bCamerafog)
			{
				*(float*)mem::FindDMAAddy(moduleBase + 0xf5400, { 0x98, 0xb68 }) = 100;
				*(float*)mem::FindDMAAddy(moduleBase + 0xf5400, { 0x98, 0xb90 }) = 100;
				std::cout << "7.Camerafog disabled.     \n";
			}
			else
			{
				*(float*)mem::FindDMAAddy(moduleBase + 0xf5400, { 0x98, 0xb68 }) = 0.5;
				*(float*)mem::FindDMAAddy(moduleBase + 0xf5400, { 0x98, 0xb90 }) = 6;
				std::cout << "7.Camerafog enabled.     \n";
			}

			setCursorPosition(0, 0);
		}
		Sleep(5);
	}

	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

void setCursorPosition(int x, int y)
{
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}
