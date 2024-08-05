#pragma once
#include "../Utils.h"
#include "Base.h"
#include "fvf.h"

namespace rage
{
    class grcIndexBuffer : datBase
    {
    public:
        uint32_t mIndexCount;
        uint16_t* mIndexData;
    };
    ASSERT_SIZE(grcIndexBuffer, 0xC);

    class grcIndexBufferD3D : grcIndexBuffer
    {
    public:
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