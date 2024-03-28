#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <intrin.h>
#include <Shlwapi.h>
#include <Psapi.h>
#include <list>
#include <ShlObj.h>
#include <filesystem>

#include "AchievementHook.h"
#include "EOSOverrider.h"

HINSTANCE g_hInstance;
const void* endAddress;

#ifdef _DEBUG
bool AchievementHook::m_ShowConsole = true;
#else
bool AchievementHook::m_ShowConsole = false;
#endif

void AchievementHook::Initialize()
{
    if (m_ShowConsole)
    {
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }
    g_hInstance = GetModuleHandle(NULL);
    MODULEINFO moduleInfo;
    GetModuleInformation(GetCurrentProcess(), g_hInstance, &moduleInfo, sizeof(MODULEINFO));
    endAddress = (const char*)g_hInstance + moduleInfo.SizeOfImage;

    WCHAR* path;
    if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path)))
    {
        fprintf(stderr, "Could not fetch local appdata folder, exiting.");
        return;
    }

    std::wstring fullDirPath(path);
    fullDirPath += L"\\EOSAchievementHook";

    if (!std::filesystem::exists(fullDirPath))
        std::filesystem::create_directory(fullDirPath);

    fprintf(stdout, "Executable instance at %p\n", g_hInstance);
    EOSOverride(g_hInstance, fullDirPath);
}