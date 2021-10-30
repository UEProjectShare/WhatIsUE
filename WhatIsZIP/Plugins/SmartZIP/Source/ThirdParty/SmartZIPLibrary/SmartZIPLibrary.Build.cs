using System.IO;
using UnrealBuildTool;

public class SmartZIPLibrary : ModuleRules
{
    public string ZIPProjectDirectory
    {
        get
        {
            return Path.GetFullPath(
                  Path.Combine(ModuleDirectory, "../../../../../")
            );
        }
    }

    private void CopyZIPDllAndLibToProjectBinaries(string Filepath, ReadOnlyTargetRules Target)
    {
        string BinariesDirectory = Path.Combine(ZIPProjectDirectory, "Binaries", Target.Platform.ToString());

        string Filename = Path.GetFileName(Filepath);

        if (!Directory.Exists(BinariesDirectory))
        {
            Directory.CreateDirectory(BinariesDirectory);
        }

        File.Copy(Filepath, Path.Combine(BinariesDirectory, Filename), true);

        RuntimeDependencies.Add(Path.Combine(BinariesDirectory, Filename));
    }

    public SmartZIPLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
			PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "x64", "Release"));
			PublicAdditionalLibraries.Add("ZIP_SDK_LIBRARY.lib");

			// Delay-load the DLL, so we can load it from the right place first
			PublicDelayLoadDLLs.Add("ZIP_SDK_LIBRARY.dll");

            CopyZIPDllAndLibToProjectBinaries(Path.Combine(ModuleDirectory,"x64", "Release", "ZIP_SDK_LIBRARY.dll"),Target);
            CopyZIPDllAndLibToProjectBinaries(Path.Combine(ModuleDirectory,"x64", "Release", "ZIP_SDK_LIBRARY.lib"), Target);
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
          //  PublicDelayLoadDLLs.Add(Path.Combine(ModuleDirectory, "Mac", "Release", "libExampleLibrary.dylib"));
        }
        else if (Target.Platform == UnrealTargetPlatform.Win32)
        {
            // Add the import library
            PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "x32", "Release"));
            PublicAdditionalLibraries.Add("ZIP_SDK_LIBRARY.lib");

            // Delay-load the DLL, so we can load it from the right place first
            PublicDelayLoadDLLs.Add("ZIP_SDK_LIBRARY.dll");

            CopyZIPDllAndLibToProjectBinaries(Path.Combine(ModuleDirectory, "x32", "Release", "ZIP_SDK_LIBRARY.dll"), Target);
            CopyZIPDllAndLibToProjectBinaries(Path.Combine(ModuleDirectory, "x32", "Release", "ZIP_SDK_LIBRARY.lib"), Target);
        }
    }
}
