#ifndef __ZIP_FILE_LIBRARY_HPP__
#define __ZIP_FILE_LIBRARY_HPP__

#include "Zipper.hpp"

namespace BloodSword::ZipFile
{
    std::string Read(zipper::UnZip &zip, const char *zipfile, const char *filename)
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
            std::string error = "Unable to load [" + std::string(filename) + "] from [" + std::string(zipfile) + "]";

            throw std::invalid_argument(error.c_str());
        }

        return buffer;
    }

    // read entire file from zip archive
    std::string Read(const char *zipfile, const char *filename)
    {
        zipper::UnZip zip(zipfile);

        auto buffer = ZipFile::Read(zip, zipfile, filename);

        zip.close();

        return buffer;
    }

    // read entire file from zip archive
    std::string Read(std::string zipfile, std::string filename)
    {
        return ZipFile::Read(zipfile.c_str(), filename.c_str());
    }
}

#endif