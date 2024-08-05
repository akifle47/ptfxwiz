#pragma once
#include "../../Utils.h"
#include "../Resource.h"
#include "PtxKeyFrame.h"
#include "../Texture.h"

namespace rage
{
    class rmPtfxShaderVar
    {
    public:
        enum class eVarType : uint8_t
        {
            BOOL = 0,
            INT,
            FLOAT,
            FLOAT2,
            FLOAT3,
            FLOAT4,
            TEXTURE,
            KEYFRAME,
            COUNT
        };

        rmPtfxShaderVar() {};

        rmPtfxShaderVar(const datResource& rsc) {};

        virtual ~rmPtfxShaderVar()
        {
            delete[] mName;
        }

        void Place(void* that, const datResource& rsc);

        int32_t field_4;
        char* mName;
        int32_t mIndex;
        int8_t field_10;
        int8_t field_11;
        eVarType mType;
        int8_t field_13[13];
    };
    ASSERT_SIZE(rmPtfxShaderVar, 0x20);


    class rmPtfxShaderVar_Bool : public rmPtfxShaderVar
    {
    public:
        rmPtfxShaderVar_Bool(const datResource& rsc) {}

        bool mValue;
        int8_t pad[15];
    };
    ASSERT_SIZE(rmPtfxShaderVar_Bool, 0x30);


    class rmPtfxShaderVar_Int : public rmPtfxShaderVar
    {
    public:
        rmPtfxShaderVar_Int(const datResource& rsc) {}

        int32_t mValue;
        int8_t pad[12];
    };
    ASSERT_SIZE(rmPtfxShaderVar_Int, 0x30);


    class rmPtfxShaderVar_Float : public rmPtfxShaderVar
    {
    public:
        rmPtfxShaderVar_Float(const datResource& rsc) {}

        float mValue;
        int8_t pad[12];
    };
    ASSERT_SIZE(rmPtfxShaderVar_Float, 0x30);


    class rmPtfxShaderVar_Float2 : public rmPtfxShaderVar
    {
    public:
        rmPtfxShaderVar_Float2(const datResource& rsc) {}

        float mValue[2];
        int8_t pad[8];
    };
    ASSERT_SIZE(rmPtfxShaderVar_Float2, 0x30);


    class rmPtfxShaderVar_Float3 : public rmPtfxShaderVar
    {
    public:
        rmPtfxShaderVar_Float3(const datResource& rsc) {}

        float mValue[3];
        int8_t pad[4];
    };
    ASSERT_SIZE(rmPtfxShaderVar_Float3, 0x30);


    class rmPtfxShaderVar_Float4 : public rmPtfxShaderVar
    {
    public:
        rmPtfxShaderVar_Float4(const datResource& rsc) {}

        float mValue[4];
    };
    ASSERT_SIZE(rmPtfxShaderVar_Float4, 0x30);


    class rmPtfxShaderVar_Texture : public rmPtfxShaderVar
    {
    public:
        rmPtfxShaderVar_Texture(const datResource& rsc)
        {
            rsc.PointerFixUp(mTexture);
            rsc.PointerFixUp(mTextureName);
        }

        ~rmPtfxShaderVar_Texture()
        {
            delete[] mTextureName;
        }

        grcTexturePC* mTexture;
        char* mTextureName;
        int8_t pad[8];
    };
    ASSERT_SIZE(rmPtfxShaderVar_Texture, 0x30);


    class rmPtfxShaderVar_Keyframe : public rmPtfxShaderVar
    {
    public:
        rmPtfxShaderVar_Keyframe(const datResource& rsc) : mKeyframe(rsc) {}

        int8_t field_20[8];
        rmPtfxKeyframe mKeyframe;
    };
    ASSERT_SIZE(rmPtfxShaderVar_Keyframe, 0x50);
}