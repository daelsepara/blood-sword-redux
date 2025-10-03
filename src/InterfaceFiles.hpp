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


// filesystem namespace alias
namespace fs = std::filesystem;

// file interface functions
namespace BloodSword::Interface
{
    std::string ConvertString(std::wstring& wstr)
    {
        // calculate required buffer size
        size_t len = wcstombs(nullptr, wstr.c_str(), 0) + 1;

        // allocate buffer
        char* buffer = new char[len];

        // perform conversion
        wcstombs(buffer, wstr.c_str(), len);

        // create std::string
        std::string str(buffer);

        // clean up
        delete[] buffer;

        return str;
    }

    // patform-dependent function for returning user document directory and save location
    std::string GetGamesPath()
    {
        auto GamesPath = std::string("/Saved Games/Blood Sword");

#if defined(_WIN32)
        PWSTR PathString;

        SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, NULL, &PathString);

        std::wstring WindowsPath(PathString);

        CoTaskMemFree(PathString);

        auto UserGamesPath = Interface::ConvertString(WindowsPath);

        UserGamesPath += GamesPath;
#else
        const char *HomeDirectory = nullptr;

        if ((HomeDirectory = getenv("HOME")) == nullptr)
        {
            HomeDirectory = getpwuid(getuid())->pw_dir;
        }

        auto UserGamesPath = std::string(HomeDirectory) + "/Documents" + GamesPath;
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
