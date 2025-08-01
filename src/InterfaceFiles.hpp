#ifndef __INTERFACE_FILES_HPP__
#define __INTERFACE_FILES_HPP__

#include <filesystem>
#include <fstream>
#include <string>

#if defined(_WIN32)
#include <windows.h>
#include <shlobj.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

namespace fs = std::filesystem;

namespace BloodSword::Interface
{
    // patform-dependent function for returning user document directory and save location
    std::string GetGamesPath()
    {
        std::string GamesPath = "/Saved Games/Blood Sword";

#if defined(_WIN32)
        PWSTR PathString;

        SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, NULL, &PathString);

        std::wstring WindowsPath(PathString);

        CoTaskMemFree(PathString);

        std::string UserGamesPath(WindowsPath.length(), ' ');

        std::copy(WindowsPath.begin(), WindowsPath.end(), UserGamesPath.begin());

        UserGamesPath += GamesPath;
#else
        const char *HomeDirectory = nullptr;

        if ((HomeDirectory = getenv("HOME")) == nullptr)
        {
            HomeDirectory = getpwuid(getuid())->pw_dir;
        }

        std::string UserGamesPath = std::string(HomeDirectory) + "/Documents" + GamesPath;
#endif

        return UserGamesPath;
    }

    // create directories
    void CreateDirectories()
    {
        auto path = Interface::GetGamesPath();

        fs::create_directories(path);
    }
}

#endif
