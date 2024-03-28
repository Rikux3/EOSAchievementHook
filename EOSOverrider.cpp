#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <map>
#include <fstream>
#include "EOSOverrider.h"
#include <vector>
#include <filesystem>

HMODULE g_eosLib = NULL;

std::wstring appdataPath;

typedef int EOS_Result;
const EOS_Result EOS_Success = 0;

struct EOS_Achievements { };

struct EOS_LocalUserId
{
    const char* UserID;
};

struct EOS_Achievements_UnlockAchievementsOptions
{
    int ApiVersion;
    void* UserId;
    const char** AchievementIds;
    unsigned int AchievementCount;
};

struct EOS_Achievements_QueryDefinitionsOptions
{
    int ApiVersion;
    EOS_LocalUserId* LocalUserId;
    void* EpicUserId_DEPRECATED;
    void* HiddenAchievementIds_DEPRECATED;
    void* HiddenAchievementsCount_DEPRECATED;
};

struct EOS_Achievements_GetAchievementDefinitionCountOptions
{
    int ApiVersion;
};

struct EOS_Achievements_DefinitionV2
{
    int ApiVersion;
    const char* AchievementId;
    const char* UnlockedDisplayName;
    const char* UnlockedDescription;
    const char* LockedDisplayName;
    const char* LockedDescription;
    const char* FlavorText;
    const char* UnlockedIconURL;
    const char* LockedIconURL;
    void* bIsHidden;
    unsigned int StatThresholdCount;
    void* StatThresholds;
};

const EOS_LocalUserId g_LocalUserID{ "" };

void EOS_Achievements_QueryDefinitions(
    EOS_Achievements* instance,
    const void* options,
    void* data,
    const void* cb)
{ }

struct EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions
{
    int ApiVersion;
    unsigned int AchievementIndex;
};

EOS_Result EOS_Achievements_QueryPlayerAchievements() { return EOS_Success; }
EOS_Result EOS_Achievements_UnlockAchievements(EOS_Achievements* instance, const EOS_Achievements_UnlockAchievementsOptions* options, void* data, const void* cb)
{
    try
    {
        std::vector<std::string> achievementIds;

        fprintf(stdout, "EOS_Achievements_UnlockAchievements called!\n");
        for (int i = 0; i < options->AchievementCount; i++)
        {
            const char* pt = options->AchievementIds[i];
            achievementIds.push_back(std::string(pt));
            fprintf(stdout, pt);
        }

        static const std::wstring filePath = appdataPath + L"\\ach.txt";
        if (std::filesystem::exists(filePath))
        {
            std::filesystem::remove(filePath);
        }

        std::ofstream out(filePath);
        for (auto& ach : achievementIds)
        {
            out << ach << std::endl;
        }
        out.close();
    }
    catch (std::exception ex)
    {
        fprintf(stderr, ex.what());
    }

    return EOS_Success; 
}

void MyFakeCallback()
{
    fprintf(stdout, "Returned from EOS_Achievements_QueryDefinitions");
}

EOS_Result EOS_Achievements_AddNotifyAchievementsUnlockedV2(EOS_Achievements* instance, const EOS_Achievements_GetAchievementDefinitionCountOptions* options, void* data, const void* cb)
{ 
    fprintf(stdout, "EOS_Achievements_AddNotifyAchievementsUnlockedV2 called!");
    //EOS_Achievements_QueryDefinitions_Call QueryDefinitions = (EOS_Achievements_QueryDefinitions_Call)GetProcAddress(library, "EOS_Achievements_QueryDefinitions");
    //EOS_Achievements_QueryDefinitionsOptions* queryOp = new EOS_Achievements_QueryDefinitionsOptions();
    //queryOp->ApiVersion = options->ApiVersion;
    //EOS_Result retVal = QueryDefinitions(instance, queryOp, 0, MyFakeCallback);

    //EOS_Achievements_GetAchievementDefinitionCount_Call GetNumOfDefs = 
    //    (EOS_Achievements_GetAchievementDefinitionCount_Call)GetProcAddress(library, "EOS_Achievements_GetAchievementDefinitionCount");
    //EOS_Achievements_GetAchievementDefinitionCountOptions* newOp = new EOS_Achievements_GetAchievementDefinitionCountOptions();
    //newOp->ApiVersion = options->ApiVersion;
    //unsigned int num = GetNumOfDefs(instance, newOp);
    //fprintf(stdout, "EOS_Achievements_GetAchievementDefinitionCount returned %d", num);

    //EOS_Achievements_CopyAchievementDefinitionV2ByIndex_Call GetThatDefinition = 
    //    (EOS_Achievements_CopyAchievementDefinitionV2ByIndex_Call)GetProcAddress(library, "EOS_Achievements_CopyAchievementDefinitionV2ByIndex");

    //EOS_Achievements_DefinitionV2_Release_Call ReleaseDefinition =
    //    (EOS_Achievements_DefinitionV2_Release_Call)GetProcAddress(library, "EOS_Achievements_DefinitionV2_Release");

    //for (unsigned int i = 0; i < num; i++)
    //{
    //    EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions* op = new EOS_Achievements_CopyAchievementDefinitionV2ByIndexOptions();
    //    op->ApiVersion = options->ApiVersion;
    //    op->AchievementIndex = i;
    //    EOS_Achievements_DefinitionV2* obj = new EOS_Achievements_DefinitionV2();
    //    GetThatDefinition(instance, op, &obj);
    //    fprintf(stdout, obj->AchievementId);
    //    ReleaseDefinition(obj);
    //}

    return EOS_Success; 
}
EOS_Result EOS_Achievements_RemoveNotifyAchievementsUnlocked() { return EOS_Success; }

std::map<std::string, void*> g_EOS_Hooks
{
    //{"EOS_Achievements_QueryDefinitions", &EOS_Achievements_QueryDefinitions},
    //{"EOS_Achievements_QueryPlayerAchievements", &EOS_Achievements_QueryPlayerAchievements},
    {"EOS_Achievements_UnlockAchievements", &EOS_Achievements_UnlockAchievements},
    //{"EOS_Achievements_AddNotifyAchievementsUnlockedV2", &EOS_Achievements_AddNotifyAchievementsUnlockedV2},
    //{"EOS_Achievements_RemoveNotifyAchievementsUnlocked", &EOS_Achievements_RemoveNotifyAchievementsUnlocked},
};

bool EOSOverride(HINSTANCE hInstance, std::wstring& appdata)
{
    fprintf(stdout, "Overriding EOS functions...\n");

    appdataPath = appdata;
    ULONG_PTR baseImage = (ULONG_PTR)hInstance;

    PIMAGE_DOS_HEADER dosHeaders = (PIMAGE_DOS_HEADER)baseImage;
    PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)(baseImage + dosHeaders->e_lfanew);

    IMAGE_DATA_DIRECTORY importsDirectory = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
    PIMAGE_IMPORT_BY_NAME functionName = NULL;

    for (
        PIMAGE_IMPORT_DESCRIPTOR importDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(baseImage + importsDirectory.VirtualAddress);
        importDescriptor->Name != NULL;
        importDescriptor++)
    {
        LPCSTR libraryName = baseImage + (LPCSTR)importDescriptor->Name;
        if (strcmp(libraryName, "EOSSDK-Win64-Shipping.dll") != 0)
            continue;

        g_eosLib = LoadLibraryA(libraryName);
        if (g_eosLib == NULL)
            return false;

        PIMAGE_THUNK_DATA originalFirstThunk = (PIMAGE_THUNK_DATA)(baseImage + importDescriptor->OriginalFirstThunk);
        PIMAGE_THUNK_DATA firstThunk = (PIMAGE_THUNK_DATA)(baseImage + importDescriptor->FirstThunk);
        while (originalFirstThunk->u1.AddressOfData != NULL)
        {
            functionName = (PIMAGE_IMPORT_BY_NAME)(baseImage + originalFirstThunk->u1.AddressOfData);
            const auto& mapPair = g_EOS_Hooks.find(std::string(functionName->Name));
            if (mapPair != g_EOS_Hooks.end())
            {
                SIZE_T bytesWritten = 0;
                DWORD oldProtect = 0;
                if (VirtualProtect((LPVOID)(&firstThunk->u1.Function), sizeof(ULONG_PTR), PAGE_READWRITE, &oldProtect))
                {
                    DWORD newProtect = 0;
                    firstThunk->u1.Function = (ULONG_PTR)mapPair->second;
                    g_EOS_Hooks.emplace(mapPair->first, mapPair->second);
                    VirtualProtect((LPVOID)(&firstThunk->u1.Function), sizeof(ULONG_PTR), oldProtect, &newProtect);
                }
            }

            originalFirstThunk++;
            firstThunk++;
        }
    }
    return true;
}
