#pragma once
#include "../Utils.h"
#include "Base.h"
#include "fvf.h"

namespace rage
{
    class grcVertexBuffer : public datBase
    {
    public:
        grcVertexBuffer(const datResource& rsc) : mFvf(rsc), mVertexData(rsc) {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mFvf.AddToLayout(layout, depth);
            layout.AddObject(mVertexData.Get(), RSC5Layout::eBlockType::PHYSICAL, mVertexCount * mStride);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mFvf.SerializePtrs(layout, rsc, depth);
            layout.SerializePtr(mVertexData.Get(), mVertexCount * mStride);
        }

        uint16_t mVertexCount;
        bool mIsLocked;
        bool mIsDynamic;
        void* mLockedData;
        uint32_t mStride;
        datOwner<grcFvf> mFvf;
        int32_t mLockThreadId;
        datOwner<void*> mVertexData;
    };
    ASSERT_SIZE(grcVertexBuffer, 0x1C);

    class grcVertexBufferD3D : public grcVertexBuffer
    {
    public:
        grcVertexBufferD3D(const datResource& rsc) : grcVertexBuffer(rsc) {}

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) grcVertexBufferD3D(rsc);
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            grcVertexBuffer::AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            grcVertexBuffer::SerializePtrs(layout, rsc, depth);
        }

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
    ASSERT_SIZE(grcVertexBufferD3D, 0x40);
}