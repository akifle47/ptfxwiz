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
        struct Entry 
        {
            union 
            {
                void* AsVoid;
                grcTexturePC* AsTexture;
                grcTextureReference* AsTextureRef;
                Vector4* AsVector4;
                Matrix44* AsMatrix44;
            };
        };

    public:
        grcInstanceData(const datResource& rsc)
        {
            if(mEntries)
                rsc.PointerFixUp(mEntries);
            if(mEntriesCounts)
                rsc.PointerFixUp(mEntriesCounts);
            if(mEntriesHashes)
                rsc.PointerFixUp(mEntriesHashes);

            for(uint32_t i = 0; i < mCount; i++)
            {
                if(!mEntries[i].AsVoid)
                    continue;

                rsc.PointerFixUp(mEntries[i].AsVoid);

                if(!mEntriesCounts[i])
                {
                    if(mEntries[i].AsTextureRef->mResourceType == grcTexture::eType::REFERENCE)
                    {
                        new(mEntries[i].AsTextureRef) grcTextureReference(rsc);
                    }
                }
            }
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            if(!mEntries)
                return;

            layout.AddObject(mEntries, RSC5Layout::eBlockType::VIRTUAL, mTotalSize);

            for(uint8_t i = 0; i < mCount; i++)
            {
                if(!mEntries[i].AsVoid)
                    continue;

                if(!mEntriesCounts[i])
                {
                    if(mEntries[i].AsTextureRef->mResourceType == grcTexture::eType::REFERENCE)
                    {
                        layout.AddObject(mEntries[i].AsTextureRef, RSC5Layout::eBlockType::VIRTUAL);
                        mEntries[i].AsTextureRef->AddToLayout(layout, depth);
                    }
                }
                else
                {
                    layout.AddObject((uint8_t*)mEntries[i].AsVoid, RSC5Layout::eBlockType::VIRTUAL);
                }
            }
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            if(!mEntries)
                return;

            uint32_t countsOffset = 4 * ((mCount + 3) & ~3u); //alligned to 4
            uint32_t hashesOffset = ((mCount + 15) & ~15) + countsOffset; //alligned to 16
            uint32_t nextEntryOffset = countsOffset + hashesOffset;

            //todo: important!! this should be done by finding the effect this instance uses then the parameter and getting the entry/param size-
            //from that, but i cant load effects yet so im using this method from LibertyFourXYZ which i assume mostly works. see 0xAC60 on patch 8
            for(uint8_t i = 0; i < mCount; i++)
            {
                if(!mEntries[i].AsVoid)
                    continue;

                switch(mEntriesCounts[i])
                {
                    case 0:
                        if(mEntries[i].AsTextureRef->mResourceType == grcTexture::eType::REFERENCE)
                        {
                            mEntries[i].AsTextureRef->SerializePtrs(layout, rsc, depth + 1);
                            layout.SerializePtr(mEntries[i].AsTextureRef, sizeof(grcTextureReference));
                        }
                    break;

                    case 1:
                    case 4:
                    case 8:
                    case 14:
                    case 15:
                    case 16:
                    {
                        uint32_t size = 0;
                        if(mEntriesCounts[i] == 8)
                            size = 16 * 6;
                        else
                            size = 16 * mEntriesCounts[i];

                        uintptr_t *entry = (uintptr_t*)((uintptr_t)mEntries + nextEntryOffset);
                        layout.SerializePtr(entry, size);
                        mEntries[i].AsVoid = entry;

                        nextEntryOffset += size;
                    }
                    break;

                    default:
                        __debugbreak();
                }
            }

            layout.SerializePtr(mEntries, mTotalSize);

            layout.BackupPtr(mEntriesCounts);
            mEntriesCounts = (uint8_t*)((uintptr_t)mEntries + countsOffset);

            layout.BackupPtr(mEntriesHashes);
            mEntriesHashes = (uint32_t*)((uintptr_t)mEntries + hashesOffset);
        }

        Entry* mEntries;
        class grcEffect *mEffect;
        uint32_t mCount;
        uint32_t mTotalSize;
        uint8_t* mEntriesCounts;
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
            layout.AddObject(mPresetName, RSC5Layout::eBlockType::VIRTUAL, strlen(mName) + 1);

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
        datOwner<void*> field_4C;
        datOwner<void*> field_50;
        int32_t mShaderPresetIndex;
        datOwner<grmShaderPresetParams> mShaderPresetParams;
    };
    ASSERT_SIZE(grmShaderFx, 0x5C);
}