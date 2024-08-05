#pragma once
#include "../Utils.h"
#include "Base.h"
#include "fvf.h"

namespace rage
{
    class grcVertexBuffer : public datBase
    {
    public:
        uint16_t mVertexCount;
        bool mIsLocked;
        bool mIsDynamic;
        void* mLockedData;
        uint32_t mStride;
        grcFvf* mFvf;
        int32_t mLockThreadId;
        void* mVertexData;
    };
    ASSERT_SIZE(grcVertexBuffer, 0x1C);

    class grcVertexBufferD3D : public grcVertexBuffer
    {
    public:
        struct IDirect3DVertexBuffer9* mVertexBufferdD3D;
        void* mStagingBuffer;
        uint32_t field_24;
        uint32_t mLockFlags;
        grcVertexBufferD3D* mNext;
        grcVertexBufferD3D* mPrev;
        int32_t field_34;
        int32_t field_38;
        int32_t field_3C;
    };
}