#pragma once
#include "../Utils.h"
#include "../Log.h"

#include <cstdint>
#include <memory>
#include <filesystem>

namespace rage
{
    struct datResourceInfo
    {
        datResourceInfo() = default;

        inline uint32_t GetVirtualChunkSize() const
        {
            return 4096 << VirtualSize;
        }

        inline uint32_t GetPhysicalChunkSize() const
        {
            return 4096 << PhysicalSize;
        }

        inline uint32_t GetVirtualSize() const
        {
            uint32_t chunkSize = GetVirtualChunkSize();
            return chunkSize * VirtualChunk0 + (VirtualChunk1 ? chunkSize / 2 : 0) + (VirtualChunk2 ? chunkSize / 4 : 0) +
                   (VirtualChunk3 ? chunkSize / 8 : 0) + (VirtualChunk4 ? chunkSize / 16 : 0);
        }

        inline uint32_t GetPhysicalSize() const
        {
            uint32_t chunkSize = GetPhysicalChunkSize();
            return chunkSize * PhysicalChunk0 + (PhysicalChunk1 ? chunkSize / 2 : 0) + (PhysicalChunk2 ? chunkSize / 4 : 0) +
                   (PhysicalChunk3 ? chunkSize / 8 : 0) + (PhysicalChunk4 ? chunkSize / 16 : 0);
        }

        inline uint8_t GetVirtualChunkCount() const
        {
            return VirtualChunk0 + VirtualChunk1 + VirtualChunk2 + VirtualChunk3 + VirtualChunk4;
        }

        inline uint8_t GetPhysicalChunkCount() const
        {
            return PhysicalChunk0 + PhysicalChunk1 + PhysicalChunk2 + PhysicalChunk3 + PhysicalChunk4;
        }

        void GenerateMap(struct datResourceMap& map) const;

        uint32_t VirtualChunk4 : 1;
        uint32_t VirtualChunk3 : 1;
        uint32_t VirtualChunk2 : 1;
        uint32_t VirtualChunk1 : 1;
        uint32_t VirtualChunk0 : 7;
        uint32_t VirtualSize   : 4;

        uint32_t PhysicalChunk4 : 1;
        uint32_t PhysicalChunk3 : 1;
        uint32_t PhysicalChunk2 : 1;
        uint32_t PhysicalChunk1 : 1;
        uint32_t PhysicalChunk0 : 7;
        uint32_t PhysicalSize   : 4;

        uint32_t IsCompressed : 1;
        uint32_t IsResource   : 1;
    };


    struct datResourceFileHeader
    {
        static constexpr uint32_t MAGIC_RSC5 = 0x05435352;

        static constexpr uint32_t VIRTUAL_BASE  = 0x50000000;
        static constexpr uint32_t PHYSICAL_BASE = 0x60000000;

        uint32_t Magic;
        uint32_t Version;
        datResourceInfo Info;
    };
    ASSERT_SIZE(datResourceFileHeader, 0xC);


    struct datResourceChunk
    {
        uint32_t SrcAddr;
        std::unique_ptr<uint8_t[]> DestAddr;
        uint32_t Size;
    };
    ASSERT_SIZE(datResourceChunk, 0xC);


    struct datResourceMap
    {
        ptrdiff_t ContainsSrc(const void* ptr) const;
        ptrdiff_t ContainsDest(const void* ptr) const;
        void FillMap(const uint8_t* resourceData);

        uint16_t VirtualCount;
        uint16_t PhysicalCount;
        datResourceChunk Chunks[128];
    };
    ASSERT_SIZE(datResourceMap, 0x604);


    struct datResource
    {
        datResource();
        datResource(const datResourceInfo& info);
        datResource(const char* name);
        datResource(const char* name, const datResourceInfo& info);

        void SaveToDisk(std::filesystem::path filePath, uint32_t version, const datResourceInfo& rscInfo) const;

        template<typename T>
        void PointerFixUp(T& ptr) const
        {
            ptr = (T)((uint8_t*)(ptr) + GetFixUp((void*)ptr));
        }

        ptrdiff_t GetFixUp(const void* ptr) const
        {
            uint32_t chunkIndex = Map->ContainsSrc(ptr);
            if(chunkIndex == -1)
            {
                Log::Error("Could not find pointer 0x%p in chunks for resource %s", ptr, *DebugName);
                return -1;
            }

            return (ptrdiff_t)Map->Chunks[chunkIndex].DestAddr.get() - Map->Chunks[chunkIndex].SrcAddr;
        }

        std::unique_ptr<datResourceMap> Map;
        std::unique_ptr<datResource> Next;
        std::unique_ptr<char*> DebugName;
        bool WasDefrag = false;
    };
    ASSERT_SIZE(datResource, 0x10);
};