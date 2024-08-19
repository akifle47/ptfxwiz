#pragma once
#include "../../Utils.h"
#include <cstdint>
#include <d3d9types.h>

namespace rage
{
    struct grcVertexDeclaration
    {
        struct IDirect3DVertexDeclaration9* DeclD3D;
        uint32_t RefCount;
        uint16_t Divider[2];
        D3DVERTEXELEMENT9 Element;
        int32_t field_14;
    };
    ASSERT_SIZE(grcVertexDeclaration, 0x18);
}