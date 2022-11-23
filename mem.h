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

public: void setup(const char* windowName, DWORD firstOffsetA);

	DWORD GetModuleBaseAddress(wchar_t* modName);

public: template<typename T> void RPM(wchar_t* modName, DWORD moduleBaseAddress, DWORD offsets[]);
public: template<typename T> void WPM(DWORD addr, T buffer);
};
