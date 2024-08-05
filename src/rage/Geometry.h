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
        grcVertexBufferD3D* mVertexBuffers[4];
        grcIndexBufferD3D* mIndexBuffers[4];
        uint32_t mIndexCount;
        uint32_t mPrimitiveCount;
        uint16_t mVertexCount;
        uint16_t mPrimitiveType;
        uint16_t* mBoneIndices;
        uint16_t mStride;
        uint16_t mBoneCount;
        grcVertexDeclaration* mInstanceVertexDeclaration;
        grcVertexBufferD3D* mInstanceVertexBuffer;
        int32_t mUseGlobalStreamIndex;
    };
    ASSERT_SIZE(grmGeometryQB, 0x4C);
}