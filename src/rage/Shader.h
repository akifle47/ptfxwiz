#pragma once
#include "Base.h"
#include "Array.h"
#include "Dictionary.h"
#include "Texture.h"
#include "Matrix.h"
#include "Vector.h"
#include "../Utils.h"

namespace rage
{
    /*
    class grcInstanceData
    {
    public:
        struct Entry 
        {
            union 
            {
                grcTexturePC* Texture;
                grcTextureReference* TextureRef;
                Vector4* Vector;
                Matrix44* Matrix;
            };
        };

        Entry* mEntries;
        grcEffect* mEffect;
        uint32_t mEntryCount;
        uint32_t mTotalEntrySize;
        uint8_t* mTypes;
        uint32_t mEffectHash;
        void* field_18;
        int32_t field_1C;
        uint32_t* mEntryHashes;
        int32_t field_24;
        int32_t field_28;
        uint32_t* mTechniqueIndices;
    };
    ASSERT_SIZE(grcInstanceData, 0x30);


    class grmShader : public datBase
    {
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


    class grmShaderFx : public grmShader
    {
    };


    class grmShaderGroup : public datBase
    {
    public:
        pgDictionary<grcTexturePC>* mTextures;
        atArray<grmShaderFx> mShaders;
        atArray<uint32_t> field_10;
        atArray<uint32_t> field_18;
        atArray<uint32_t> field_20;
        atArray<uint32_t> field_28;
        atArray<uint32_t> field_30;
        atArray<uint32_t> field_38;
        atArray<uint32_t> mVertexFormats;
        atArray<uint32_t> mIndexMappings;
    };
    ASSERT_SIZE(grmShaderGroup, 0x50);
    */
}