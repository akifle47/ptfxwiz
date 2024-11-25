#pragma once
#include "../Base.h"
#include "../Dictionary.h"
#include "../grcore/TextureReference.h"
#include "../grcore/Effect.h"

namespace rage
{
    class grmShader : public pgBase
    {
    public:
        grmShader(const datResource& rsc) : mInstanceData(rsc) {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            pgBase::AddToLayout(layout, depth);
            mInstanceData.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            pgBase::SerializePtrs(layout, rsc, depth);
            mInstanceData.SerializePtrs(layout, rsc, depth);
        }

        uint8_t mVersion;
        uint8_t mDrawBucket;

        union
        {
            uint8_t mHasPreset : 1;
            uint8_t mUnknown : 7;
        };
        int8_t field_B;
        int16_t field_C;
        uint16_t mEffectIndex;
        int32_t field_10;
        grcInstanceData mInstanceData;
    };
    ASSERT_SIZE(grmShader, 0x44);


    struct grmShaderPresetParams
    {
    public:
       enum class eValueType : uint32_t
       {
            INT = 1,
            FLOAT,
            VECTOR2,
            VECTOR3,
            VECTOR4,
            TEXTURE,
            BOOL,
            MATRIX34,
            MATRIX44,
       };

    public:
        grmShaderPresetParams(const datResource& rsc) : mNext(rsc)
        {
            if(mName)
                rsc.PointerFixUp(mName);

            if(mVoid)
            {
                if(mType == eValueType::TEXTURE || mType == eValueType::MATRIX34 || mType == eValueType::MATRIX44)
                {
                    rsc.PointerFixUp(mVoid);
                }
            }
        }

        void Place(void* that, const datResource& rsc)
        {
            new(that) grmShaderPresetParams(rsc);
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mNext.AddToLayout(layout, depth);

            layout.AddObject(mName, RSC5Layout::eBlockType::VIRTUAL, strlen(mName) + 1);

            if(mVoid)
            {
                if(mType == eValueType::TEXTURE || mType == eValueType::MATRIX34 || mType == eValueType::MATRIX44)
                {
                    layout.AddObject((uint32_t*)mVoid, RSC5Layout::eBlockType::VIRTUAL);
                }
            }
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mNext.SerializePtrs(layout, rsc, depth);

            layout.SerializePtr(mName, strlen(mName) + 1);

            if(mVoid)
            {
                if(mType == eValueType::TEXTURE || mType == eValueType::MATRIX34 || mType == eValueType::MATRIX44)
                {
                    layout.SerializePtr(mVoid, sizeof(uintptr_t*));
                }
            }
        }

        union
        {
            void* mVoid;
            int32_t mInt;
            float mFloat;
            Vector2 mVector2;
            Vector3 mVector3;
            Vector4 mVector4;
            grcTexture* mTexture;
            bool mBool;
            Matrix34* mMatrix34;
            Matrix44* mMatrix44;
        };
        char *mName;
        eValueType mType;
        datOwner<grmShaderPresetParams> mNext;
        bool field_1C;
        int8_t field_1D[3];
        uint32_t mIndex;
    };
    ASSERT_SIZE(grmShaderPresetParams, 0x24);


    class grmShaderFx : public grmShader
    {
    public:
        grmShaderFx(const datResource& rsc) : grmShader(rsc), field_4C(rsc), field_50(rsc), mShaderPresetParams(rsc)
        {
            if(mName)
                rsc.PointerFixUp(mName);
            if(mPresetName)
                rsc.PointerFixUp(mPresetName);
        }

        void Place(void* that, const datResource& rsc)
        {
            new(that) grmShaderFx(rsc);
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            grmShader::AddToLayout(layout, depth);

            field_4C.AddToLayout(layout, depth);
            field_50.AddToLayout(layout, depth);

            layout.AddObject(mName, RSC5Layout::eBlockType::VIRTUAL, strlen(mName) + 1);
            layout.AddObject(mPresetName, RSC5Layout::eBlockType::VIRTUAL, strlen(mPresetName) + 1);

            mShaderPresetParams.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            grmShader::SerializePtrs(layout, rsc, depth);

            field_4C.SerializePtrs(layout, rsc, depth);
            field_50.SerializePtrs(layout, rsc, depth);

            layout.SerializePtr(mName, strlen(mName) + 1);
            layout.SerializePtr(mPresetName, strlen(mPresetName) + 1);

            mShaderPresetParams.SerializePtrs(layout, rsc, depth);
        }

        char* mName;
        char* mPresetName;
        //unknown type - always null
        datOwner<void*> field_4C;
        //unknown type - always null
        datOwner<void*> field_50;
        int32_t mShaderPresetIndex;
        datOwner<grmShaderPresetParams> mShaderPresetParams;
    };
    ASSERT_SIZE(grmShaderFx, 0x5C);
}