#pragma once
#include "../Utils.h"
#include "Array.h"
#include "Base.h"
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