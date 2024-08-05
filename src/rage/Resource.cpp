#include "Resource.h"
#include "../Log.h"
#include "zlib/zlib.h"

#include <fstream>

//============= datResourceInfo
void rage::datResourceInfo::GenerateMap(datResourceMap& map) const
{
    map.VirtualCount  = GetVirtualChunkCount();
    map.PhysicalCount = GetPhysicalChunkCount();

    uint32_t base = datResourceFileHeader::VIRTUAL_BASE;
    datResourceChunk* chunk = map.Chunks;
    uint32_t chunkSize = GetVirtualChunkSize();
    uint32_t totalSize = GetVirtualSize();

    for(uint32_t i = 0; i < GetVirtualChunkCount(); i++, chunk++) 
    {
        while (chunkSize > totalSize) chunkSize >>= 1;
        chunk->SrcAddr  = base;
        chunk->DestAddr = nullptr;
        chunk->Size     = chunkSize;

        base      += chunkSize;
        totalSize -= chunkSize;
    }

    base = datResourceFileHeader::PHYSICAL_BASE;
    chunkSize = GetPhysicalChunkSize();
    totalSize = GetPhysicalSize();
    for(uint32_t i = 0; i < GetPhysicalChunkCount(); i++, chunk++) 
    {
        while (chunkSize > totalSize) chunkSize >>= 1;
        chunk->SrcAddr  = base;
        chunk->DestAddr = nullptr;
        chunk->Size     = chunkSize;

        base      += chunkSize;
        totalSize -= chunkSize;
    }


    chunk = map.Chunks;
    for(uint32_t i = 0; i < uint32_t(map.VirtualCount + map.PhysicalCount); i++) 
    {
        chunk->DestAddr = std::make_unique<uint8_t[]>(chunk->Size);
        memset(chunk->DestAddr.get(), 0xCD, chunk->Size);
        chunk++;
    }
}
//============= datResourceMap
ptrdiff_t rage::datResourceMap::ContainsSrc(const void* ptr) const
{
    uint32_t chunkCount = VirtualCount + PhysicalCount;
    uint32_t chunkIndex = 0;
    for(const datResourceChunk* chunk = Chunks; (uint32_t)ptr < chunk->SrcAddr || (uint32_t)ptr >= chunk->SrcAddr + chunk->Size; chunk++)
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
    for(const datResourceChunk* chunk = Chunks; 
       (uint32_t)ptr < (uint32_t)chunk->DestAddr.get() || (uint32_t)ptr >= (uint32_t)chunk->DestAddr.get() + chunk->Size; chunk++)
    {
        if(chunkIndex >= chunkCount)
        {
            return -1;
        }
    };

    return chunkIndex;
}

void rage::datResourceMap::FillMap(const uint8_t* resourceData)
{
    uint32_t pos = 0;
    datResourceChunk* chunk = Chunks;
    for(uint32_t i = 0; i < uint32_t(VirtualCount + PhysicalCount); i++)
    {
        memmove(chunk->DestAddr.get(), resourceData + pos, chunk->Size);
        pos += chunk->Size;
        chunk++;
    }
}


//============= datResource
rage::datResource::datResource()
{
    Map = std::make_unique<datResourceMap>();
    Next = nullptr;
    DebugName = std::make_unique<char*>(new char[strlen("Unnamed")]);
    strcpy(*DebugName, "Unnamed");
}

rage::datResource::datResource(const datResourceInfo& info)
{
    Map = std::make_unique<datResourceMap>();
    Next = nullptr;
    DebugName = std::make_unique<char*>(new char[strlen("Unnamed")]);
    strcpy(*DebugName, "Unnamed");

    info.GenerateMap(*Map);
}

rage::datResource::datResource(const char* name)
{
    Map = std::make_unique<datResourceMap>();
    Next = nullptr;
    DebugName = std::make_unique<char*>(_strdup(name));
}

rage::datResource::datResource(const char* name, const datResourceInfo& info)
{
    Map = std::make_unique<datResourceMap>();
    Next = nullptr;
    DebugName = std::make_unique<char*>(_strdup(name));

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
        memcpy(&uncompressedData[pos], chunk.DestAddr.get(), chunk.Size);
        pos += chunk.Size;
    }

    uLongf compressedSize = totalRscSize;
    compress(compressedData.data(), &compressedSize, uncompressedData.data(), totalRscSize);

    std::ofstream file(filePath, std::ios::binary);
    if(!file.good())
    {
        Log::Error("Failed to save resource %s.", filePath.string().c_str());
        return;
    }

    file.write((char*)&datResourceFileHeader::MAGIC_RSC5, 4);
    file.write((char*)&version, 4);
    file.write((char*)&rscInfo, sizeof(rscInfo));
    file.write((char*)compressedData.data(), compressedSize);
}