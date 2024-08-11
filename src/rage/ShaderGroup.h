#pragma once
#include "Base.h"
#include "Dictionary.h"
#include "Array.h"
#include "Shader.h"

namespace rage
{
    class grmShaderGroup_obj1
    {
        uint8_t field_0[0x8];
    };
    ASSERT_SIZE(grmShaderGroup_obj1, 0x8);

    class grmShaderGroup : public datBase
    {
    public:
        grmShaderGroup(const datResource& rsc) : mTextures(rsc), mShaders(rsc), field_10(rsc), field_18(rsc),field_20(rsc), field_28(rsc), 
                                                 field_30(rsc), field_38(rsc), mVertexFormats(rsc), mIndexMappings(rsc) {}

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) grmShaderGroup(rsc);
        }

        datOwner<pgDictionary<grcTexturePC>> mTextures;
        atArray<datOwner<grmShader>> mShaders;
        //todo: unknown types
        atArray<datOwner<void*>> field_10;
        atArray<datOwner<void*>> field_18;
        atArray<datOwner<void*>> field_20;
        atArray<datOwner<void*>> field_28;
        atArray<uint32_t> field_30;

        atArray<grmShaderGroup_obj1> field_38;
        atArray<uint32_t> mVertexFormats;
        atArray<uint32_t> mIndexMappings;
    };
    ASSERT_SIZE(grmShaderGroup, 0x50);

}