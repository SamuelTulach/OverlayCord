#include "OverlayCord.h"
#include <iostream>

void Error()
{
    getchar();
    exit(-1);
}

int main()
{
    printf("[>] Searching for target window...\n");
	HWND targetWindow = FindWindowA("UnityWndClass", "Kerbal Space Program");
    if (!targetWindow)
    {
        printf("[!] Target window not found\n");
        Error();
    }

    printf("[+] Target window found with HWND 0x%p\n", targetWindow);

    printf("[>] Resolving process ID...\n");
    UINT targetProcessId;
    GetWindowThreadProcessId(targetWindow, reinterpret_cast<LPDWORD>(&targetProcessId));
    if (!targetProcessId)
    {
        printf("[!] Failed to resolve process ID\n");
        Error();
    }

    printf("[+] Process ID resolved to %u\n", targetProcessId);

    getchar();
    return 0;
}

