#pragma once
#include "Base.h"
#include "Array.h"
#include "Dictionary.h"
#include "TextureReference.h"
#include "Matrix.h"
#include "Vector.h"
#include "../Utils.h"

namespace rage
{
    class grcInstanceData
    {
    public:
        grcInstanceData(const datResource& rsc)
        {
            if(mEntries)
                rsc.PointerFixUp(mEntries);    
            if(mDataCounts)
                rsc.PointerFixUp(mDataCounts);    
            if(mEntriesHashes)
                rsc.PointerFixUp(mEntriesHashes);

            for(uint32_t i = 0; i < mCount; i++)
            {
                rsc.PointerFixUp(mEntries[i].Data);

                if(!mDataCounts[i])
                {
                    if(mEntries[i].Texture->mResourceType == grcTexture::eType::REFERENCE)
                    {
                        new(mEntries[i].TextureRef) grcTextureReference(rsc);
                    }
                }
            }
        }

        struct Entry 
        {
            union 
            {
                void* Data;
                grcTexturePC* Texture;
                grcTextureReference* TextureRef;
                Vector4* Vector;
                Matrix44* Matrix;
            };
        };

        Entry* mEntries;
        class grcEffect *mEffect;
        uint32_t mCount;
        uint32_t mTotalSize;
        uint8_t* mDataCounts;
        uint32_t mEffectHash;
        void* field_18;
        uint32_t field_1C;
        uint32_t* mEntriesHashes;
        class grcEffect* mNextEffect;
        uint32_t field_28;
        uint8_t* mTechNameIndices;
    };
    ASSERT_SIZE(grcInstanceData, 0x30);


    class grmShader : public pgBase
    {
    public:
        grmShader(const datResource& rsc) : mInstanceData(rsc) {}

        void Place(void* that, const datResource& rsc);

        uint8_t mVersion;
        uint8_t mDrawBucket;

        union 
        {
            struct 
            {
                uint8_t mHasPreset : 1;
                uint8_t mUnknown : 7;
            };

            uint8_t mFlags;
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

            if(*(uint32_t*)mValue)
            {
                if(mType == eValueType::TEXTURE || mType == eValueType::MATRIX34 || mType == eValueType::MATRIX44)
                {
                    rsc.PointerFixUp(*(uint32_t*)mValue);
                }
            }
        }

        void Place(void* that, const datResource& rsc)
        {
            new(that) grmShaderPresetParams(rsc);
        }

        //todo: make this an union with every type in eValueType
        uint8_t mValue[16];
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
            {
                rsc.PointerFixUp(mPresetName);
            }
        }

        char* mName;
        char* mPresetName;
        datOwner<void*> field_4C;
        datOwner<void*> field_50;
        int32_t mShaderPresetIndex;
        datOwner<grmShaderPresetParams> mShaderPresetParams;
    };
    ASSERT_SIZE(grmShaderFx, 0x5C);
}