# EOSAchievementHook

This library hooks into `EOSSDK-Win64-Shipping.dll` and redirects `EOS_Achievements_UnlockAchievements` to write the results of that method into a text file. This was initially developed to re-enable game achievements for already beaten games via self-made achievement trackers (e.g for speedrunning).

I can confirm it working on `EOSSDK-Win64-Shipping.dll` version 1.10.3.0.

## Wait, why a text file?
My initial plan was to use socket connections for communication, but because C++ isn't my strong suit, i've settled for plain text files.

The results of the hook will be written to `%LOCALAPPDATA%/EOSAchievementHook`.

# Does Game X work with this?
Any game distributed on EGS that loads helper DLLs like `DBGHELP.DLL` or `DINPUT8.DLL` should technically work with this library, but i've only checked with one certain SDK version.

# How Do I Use This?
Put `DBGHELP.DLL` next to the executable and start the game through EGS.

# Credits
This could not have been possible without the awesome work of the OpenKH project, in particular from Xeeynamo and MainMemory. Some remnants of a EOS hook might be hidden in OpenKH's commit history from which i've derived this project...
That said, the previously mentioned parties aren't directly involved in this project, so please don't bother them.