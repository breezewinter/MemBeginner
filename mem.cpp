#include "mem.h"

void mem::setup(const char* windowName, DWORD firstOffsetA)
{
	window = FindWindowA(windowName, nullptr);
	GetWindowThreadProcessId(window, &procId);
	procHandle = OpenProcess(PROCESS_ALL_ACCESS, false, procId);
    baseAddress = GetModuleBaseAddress(modName);
    firstOffset = firstOffsetA;
}

DWORD mem::GetModuleBaseAddress(wchar_t* modName)
{
    //initialize to zero for error checking
    DWORD modBaseAddr = 0;

    //get a handle to a snapshot of all modules
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

    //check if it's valid
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        //this struct holds the actual module information
        MODULEENTRY32 modEntry;

        //this is required for the function to work
        modEntry.dwSize = sizeof(modEntry);

        //If a module exists, get it's entry
        if (Module32First(hSnap, &modEntry))
        {
            //loop through the modules
            do
            {
                //compare the module name against ours
                if (!_wcsicmp(modEntry.szModule, modName))
                {
                    //copy the base address and break out of the loop
                    modBaseAddr = (DWORD)modEntry.modBaseAddr;
                    break;
                }

                //each iteration we grab the next module entry
            } while (Module32Next(hSnap, &modEntry));
        }
    }

    //free the handle
    CloseHandle(hSnap);
    return modBaseAddr;
}

template<typename T> void mem::RPM(wchar_t* modName, DWORD moduleBaseAddress, DWORD offsets[])
{
    baseAddress = GetModuleBaseAddress(modName);
    DWORD addr = baseAddress + firstOffset;
    ReadProcessMemory(procHandle, (void*)addr, &addr, sizeof(addr), nullptr);

    for (int i = 0; i < sizeof(*(offsets)) - 1; i++)
    {
        addr += offsets[i];
        ReadProcessMemory(procHandle, (void*)addr, &addr, sizeof(addr), nullptr);
    }

    T buffer;
    ReadProcessMemory(procHandle, (void*)addr, &buffer, sizeof(T), nullptr);

    return buffer;
}

template<typename T> void mem::WPM(DWORD addr, T buffer)
{
    WriteProcessMemory(procHandle, (void*)addr, &buffer, sizeof(buffer), nullptr);
}
