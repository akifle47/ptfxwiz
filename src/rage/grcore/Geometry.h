#pragma once
#include "../../Utils.h"
#include "../Array.h"
#include "../Base.h"
#include "VertexDecl.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace rage
{
    class grmGeometry : public datBase
    {
    public:
        grcVertexDeclaration* mDeclaration;
        int32_t mType;
    };
    ASSERT_SIZE(grmGeometry, 0xC);


    class grmGeometryQB : public grmGeometry
    {
    public:
        grmGeometryQB(const datResource& rsc) : mVertexBuffers{rsc, rsc, rsc, rsc}, mIndexBuffers{rsc, rsc, rsc, rsc},
                                                mInstanceVertexBuffer(rsc), mBoneIndices(rsc) {}

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) grmGeometryQB(rsc);
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            for(uint8_t i = 0; i < 4; i++)
            {
                mVertexBuffers[i].AddToLayout(layout, depth);
                mIndexBuffers[i].AddToLayout(layout, depth);
            }

            mInstanceVertexBuffer.AddToLayout(layout, depth);

            if(mBoneCount)
            {
                layout.AddObject(mBoneIndices.Get(), RSC5Layout::eBlockType::VIRTUAL, mBoneCount);
            }
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            for(uint8_t i = 0; i < 4; i++)
            {
                mVertexBuffers[i].SerializePtrs(layout, rsc, depth);
                mIndexBuffers[i].SerializePtrs(layout, rsc, depth);
            }

            mInstanceVertexBuffer.SerializePtrs(layout, rsc, depth);
            mBoneIndices.SerializePtrs(layout, rsc, depth);
        }

        datOwner<grcVertexBufferD3D> mVertexBuffers[4];
        datOwner<grcIndexBufferD3D> mIndexBuffers[4];
        uint32_t mIndexCount;
        uint32_t mPrimitiveCount;
        uint16_t mVertexCount;
        uint16_t mPrimitiveType;
        datOwner<uint16_t> mBoneIndices;
        uint16_t mStride;
        uint16_t mBoneCount;
        grcVertexDeclaration* mInstanceVertexDeclaration;
        datOwner<grcVertexBufferD3D> mInstanceVertexBuffer;
        int32_t mUseGlobalStreamIndex;
    };
    ASSERT_SIZE(grmGeometryQB, 0x4C);
}