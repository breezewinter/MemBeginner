#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

class mem
{
	HWND window;

	DWORD procId = NULL;

	HANDLE procHandle;

	DWORD baseAddress;
	DWORD firstOffset;

	void setup(const char* windowName, wchar_t* modName, DWORD firstOffsetA);

	DWORD GetModuleBaseAddress(wchar_t* modName);

	template<typename T> void RPM(DWORD moduleBaseAddress, DWORD offsets[]);
	template<typename T> void WPM(DWORD addr, T buffer);
};

