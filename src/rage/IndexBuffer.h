#pragma once
#include "../Utils.h"
#include "Base.h"
#include "fvf.h"

namespace rage
{
    class grcIndexBuffer : datBase
    {
    public:
        grcIndexBuffer(const datResource& rsc) : mIndexData(rsc) {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            layout.AddObject(mIndexData.Get(), RSC5Layout::eBlockType::PHYSICAL, mIndexCount);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            layout.SerializePtr(mIndexData.Get(), sizeof(mIndexCount) * mIndexCount);
        }

        uint32_t mIndexCount;
        datOwner<uint16_t> mIndexData;
    };
    ASSERT_SIZE(grcIndexBuffer, 0xC);


    class grcIndexBufferD3D : grcIndexBuffer
    {
    public:
        grcIndexBufferD3D(const datResource& rsc) : grcIndexBuffer(rsc) {}

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) grcIndexBufferD3D(rsc);
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            grcIndexBuffer::AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            grcIndexBuffer::SerializePtrs(layout, rsc, depth);
        }

        struct IDirect3DIndexBuffer9* mIndexBufferD3D;
        void* mStagingBuffer;
        uint32_t mLockFlags;
        int32_t field_18;
        int32_t field_1C;
        grcIndexBufferD3D* mNext;
        grcIndexBufferD3D* mPrev;
        int32_t field_28;
        int32_t field_2C;
    };
    ASSERT_SIZE(grcIndexBufferD3D, 0x30);
}