#include <iostream>
#include <windows.h>
using namespace std;

ULONG_PTR (__stdcall *GH_SetHook)(PVOID TargetFunc, PVOID OurFunc);
ULONG_PTR(__stdcall* GH_UnHook)(PVOID TargetFunc, PVOID OurFunc);

decltype(auto) OriCreateFileW = &CreateFile;

HANDLE
WINAPI
DetourCreateFileW(
    _In_ LPCWSTR lpFileName,
    _In_ DWORD dwDesiredAccess,
    _In_ DWORD dwShareMode,
    _In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    _In_ DWORD dwCreationDisposition,
    _In_ DWORD dwFlagsAndAttributes,
    _In_opt_ HANDLE hTemplateFile
) {
    wprintf(lpFileName);
    return OriCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

int main()
{
    HMODULE h = LoadLibrary(L"ghhlp64.dll");
    if (!h)
        wprintf(L"[+]cant find ghhlp64.dll\n");

    GH_SetHook = (decltype(GH_SetHook))GetProcAddress(h, "GH_SetHook");
    GH_UnHook = (decltype(GH_UnHook))GetProcAddress(h, "GH_UnHook");


    GH_SetHook(&OriCreateFileW, DetourCreateFileW);

    CreateFile(L"123", 1, 2, NULL, 3, 4, (HANDLE)5);



    getchar();
    return 0;
}

