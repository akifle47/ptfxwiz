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

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mTextures.AddToLayout(layout, depth);
            mShaders.AddToLayout(layout, depth);

            field_10.AddToLayout(layout, depth);
            field_18.AddToLayout(layout, depth);
            field_20.AddToLayout(layout, depth);
            field_28.AddToLayout(layout, depth);
            field_30.AddToLayout(layout, depth);

            field_38.AddToLayout(layout, depth);
            mVertexFormats.AddToLayout(layout, depth);
            mIndexMappings.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mTextures.SerializePtrs(layout, rsc, depth);
            mShaders.SerializePtrs(layout, rsc, depth);

            field_10.SerializePtrs(layout, rsc, depth);
            field_18.SerializePtrs(layout, rsc, depth);
            field_20.SerializePtrs(layout, rsc, depth);
            field_28.SerializePtrs(layout, rsc, depth);
            field_30.SerializePtrs(layout, rsc, depth);

            field_38.SerializePtrs(layout, rsc, depth);
            mVertexFormats.SerializePtrs(layout, rsc, depth);
            mIndexMappings.SerializePtrs(layout, rsc, depth);
        }

        datOwner<pgDictionary<grcTexturePC>> mTextures;
        atArray<datOwner<grmShaderFx>> mShaders;
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