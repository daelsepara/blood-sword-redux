#ifndef __ZIP_FILE_LIBRARY_HPP__
#define __ZIP_FILE_LIBRARY_HPP__

#include "Zipper.hpp"

namespace BloodSword::ZipFile
{
    // open zip archive
    zipper::UnZip Open(const char *zip_file)
    {
        auto zip = zipper::UnZip(zip_file);

        return zip;
    }

    // close zip archive
    void Close(zipper::UnZip &zip)
    {
        zip.close();
    }

    // read filename from open zip archive
    std::string Read(zipper::UnZip &zip, const char *zip_file, const char *filename)
    {
        auto buffer = std::string();

        try
        {
            // read the specified file into a string
            if (zip.next(filename))
            {
                zip.read(buffer);
            }
        }
        catch (std::exception &e)
        {
            std::string error = "Unable to load [" + std::string(filename) + "] from [" + std::string(zip_file) + "]";

            throw std::invalid_argument(error.c_str());
        }

        return buffer;
    }

    // read entire file from zip archive
    std::string Read(const char *zip_file, const char *filename)
    {
        auto zip = zipper::UnZip(zip_file);

        auto buffer = ZipFile::Read(zip, zip_file, filename);

        zip.close();

        return buffer;
    }

    // read entire file from zip archive
    std::string Read(std::string zip_file, std::string filename)
    {
        return ZipFile::Read(zip_file.c_str(), filename.c_str());
    }
}

#endif