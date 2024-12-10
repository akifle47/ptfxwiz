#include "Resource.h"
#include "../Log.h"
#include "zlib/zlib.h"

#include <fstream>

//============= datResourceInfo
void rage::datResourceInfo::GenerateMap(datResourceMap& map) const
{
    map.VirtualCount  = GetVirtualChunkCount();
    map.PhysicalCount = GetPhysicalChunkCount();

    datResourceChunk* chunk = map.Chunks;

    uint32_t base = datResourceFileHeader::VIRTUAL_BASE;
    uint32_t size = GetVirtualSize();
    uint32_t chunkSize = GetVirtualChunkSize();
    uint32_t dest = (uint32_t)new uint8_t[size];
    memset((void*)dest, 0xCD, size);

    for(uint32_t i = 0; i < GetVirtualChunkCount(); i++, chunk++)
    {
        while(chunkSize > size) chunkSize >>= 1;
        chunk->SrcAddr = base;
        chunk->DestAddr = dest;
        chunk->Size     = chunkSize;

        base += chunkSize;
        dest += chunkSize;
        size -= chunkSize;
    }

    base = datResourceFileHeader::PHYSICAL_BASE;
    size = GetPhysicalSize();
    chunkSize = GetPhysicalChunkSize();
    dest = (uint32_t)new uint8_t[size];
    memset((void*)dest, 0xCD, size);

    for(uint32_t i = 0; i < GetPhysicalChunkCount(); i++, chunk++) 
    {
        while(chunkSize > size) chunkSize >>= 1;
        chunk->SrcAddr  = base;
        chunk->DestAddr = dest;
        chunk->Size     = chunkSize;

        dest   += chunkSize;
        base   += chunkSize;
        size   -= chunkSize;
    }
}
//============= datResourceMap
ptrdiff_t rage::datResourceMap::ContainsSrc(const void* ptr) const
{
    uint32_t chunkCount = VirtualCount + PhysicalCount;
    uint32_t chunkIndex = 0;
    for(const datResourceChunk* chunk = Chunks; (uint32_t)ptr < chunk->SrcAddr || (uint32_t)ptr >= chunk->SrcAddr + chunk->Size; ++chunk)
    {
        if(++chunkIndex >= chunkCount)
        {
            return -1;
        }
    };

    return chunkIndex;
}

ptrdiff_t rage::datResourceMap::ContainsDest(const void* ptr) const
{
    uint32_t chunkCount = VirtualCount + PhysicalCount;
    uint32_t chunkIndex = 0;
    for(const datResourceChunk* chunk = Chunks; (uint32_t)ptr < chunk->DestAddr || (uint32_t)ptr >= chunk->DestAddr + chunk->Size; ++chunk)
    {
        if(++chunkIndex >= chunkCount)
        {
            return -1;
        }
    };

    return chunkIndex;
}

void rage::datResourceMap::FreeAllChunks()
{
    delete[] (uint8_t*)Chunks[0].DestAddr;
    delete[] (uint8_t*)Chunks[VirtualCount].DestAddr;
}

void rage::datResourceMap::FillMap(const uint8_t* resourceData)
{
    uint32_t pos = 0;
    datResourceChunk* chunk = Chunks;
    for(uint32_t i = 0; i < uint32_t(VirtualCount + PhysicalCount); i++)
    {
        memmove((void*)chunk->DestAddr, resourceData + pos, chunk->Size);
        pos += chunk->Size;
        chunk++;
    }
}


//============= datResource
rage::datResource::datResource() : Map(std::make_unique<datResourceMap>()),  Next(nullptr), DebugName{"Unnamed"}
{}

rage::datResource::datResource(const datResourceInfo& info) : Map(std::make_unique<datResourceMap>()), Next(nullptr), DebugName{"Unnamed"}
{
    info.GenerateMap(*Map);
}

rage::datResource::datResource(const char* name) : Map(std::make_unique<datResourceMap>()), Next(nullptr), DebugName(name)
{}

rage::datResource::datResource(const char* name, const datResourceInfo& info) : Map(std::make_unique<datResourceMap>()), Next(nullptr), DebugName(name)
{
    info.GenerateMap(*Map);
}

void rage::datResource::SaveToDisk(std::filesystem::path filePath, uint32_t version, const datResourceInfo& rscInfo) const
{
    const uLong totalRscSize = rscInfo.GetVirtualSize() + rscInfo.GetPhysicalSize();
    std::vector<uint8_t> uncompressedData(totalRscSize);
    std::vector<uint8_t> compressedData  (totalRscSize);
    
    uint32_t pos = 0;
    for(uint32_t i = 0; i < uint32_t(Map->VirtualCount + Map->PhysicalCount); i++)
    {
        const datResourceChunk& chunk = Map->Chunks[i];
        memcpy(&uncompressedData[pos], (void*)chunk.DestAddr, chunk.Size);
        pos += chunk.Size;
    }

    uLongf compressedSize = totalRscSize;
    compress(compressedData.data(), &compressedSize, uncompressedData.data(), totalRscSize);

    if(!std::filesystem::exists(filePath.parent_path()))
        std::filesystem::create_directories(filePath.parent_path());

    std::ofstream file(filePath, std::ios::binary);
    if(!file.good())
    {
        Log::Error("Failed to save resource %s.", filePath.string().c_str());
        return;
    }

    file.write((char*)&datResourceFileHeader::MAGIC_RSC5, 4);
    file.write((char*)&version, 4);
    file.write((char*)&rscInfo, 4);
    file.write((char*)compressedData.data(), compressedSize);
}