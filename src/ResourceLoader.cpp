#include "ResourceLoader.h"
#include "AString.h"

#include <fstream>
#include <zlib/zlib.h>

bool ResourceLoader::Load(std::filesystem::path filePath, uint32_t expectedVersion, rage::datResource* rscOut, rage::datResourceInfo* infoOut)
{
    AString absolutePath(std::filesystem::absolute(filePath).string().c_str());

    if(!rscOut)
    {
        Log::Error("rscOut must not be nullptr.");
        return false;
    }

    if(!std::filesystem::exists(filePath))
    {
        Log::Error("File \"%s\" does not exist.", absolutePath.Get());
        return false;
    }

    std::ifstream file(filePath, std::ios::binary);
    if(!file.good())
    {
        Log::Error("Unable to open resource file \"%s\".", absolutePath.Get());
        return false;
    }

    const rage::datResourceFileHeader rscHeader {};
    file.read((char*)&rscHeader, sizeof(rage::datResourceFileHeader));
    if(infoOut)
    {
        *infoOut = rscHeader.Info;
    }

    if(rscHeader.Magic != rage::datResourceFileHeader::MAGIC_RSC5)
    {
        Log::Error("File \"%s\" uses an unsupported version or is not a resource file.", absolutePath.Get());
        return false;
    }

    if(rscHeader.Version != expectedVersion)
    {
        Log::Error("Failed to load resource file \"%s\" - Expected version %d but the resource uses version %d.", absolutePath.Get(), expectedVersion, rscHeader.Version);
        return false;
    };

    uintmax_t fileSize = std::filesystem::file_size(filePath);
    std::vector<uint8_t> rscData(rscHeader.Info.GetVirtualSize() + rscHeader.Info.GetPhysicalSize());

    if(rscHeader.Info.IsCompressed)
    {
        uLongf uncompressedSize = (uLongf)rscData.size();

        std::vector<uint8_t> compressedData((uint32_t)fileSize - sizeof(rage::datResourceFileHeader));
        file.read((char*)compressedData.data(), compressedData.size());

        uncompress(rscData.data(), &uncompressedSize, compressedData.data(), (uLongf)rscData.size());
    }
    else
    {
        return false;
    }

    rscHeader.Info.GenerateMap(*rscOut->Map);
    rscOut->Map->FillMap(rscData.data());

    return true;
}