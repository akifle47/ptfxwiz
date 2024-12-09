#pragma once
#include "../Base.h"
#include "../Resource.h"
#include "../../DDS.h"

#include <d3d9.h>

namespace rage
{
    enum grcTextureFormat : uint8_t
    {
        grctfUNKNOWN        = 0,
        grctfR5G6B5         = 1,
        grctfA8R8G8B8       = 2,
        grctfR16F           = 3,
        grctfR32F           = 4,
        grctfA2R10G10B10    = 5,
        grctfA16B16G16R16F  = 6,
        grctfG16R16         = 7,
        grctfG16R16F        = 8,
        grctfA32B32G32R32F  = 9,
        grctfA16B16G16R16F_ = 10,
        grctfA16B16G16R16   = 11,
        grctfL8             = 12,
        grctfA1R5G5B5       = 13,
        grctfD24S8          = 14,
        grctfD32F_LOCKABLE  = 15,
        grctfX8R8G8B8       = 16,
        grctfR16F_          = 17,
        grctfG32R32F        = 18,
    };

    enum class eTextureType : uint8_t
    {
        STANDARD = 0,
        //dont think these are ever used or even properly supported
        CUBE     = 1,
        DEPTH    = 2,
        VOLUME   = 2,
    };

    class grcTexture : public pgBase
    {
    public:
        enum class eType : uint8_t
        {
            STANDARD = 0,
            RENDER_TARGET,
            REFERENCE,
            COUNT,
        };

    public:
        grcTexture() : mResourceType(eType::STANDARD), mLayerCount(0), mRefCount(1), field_C(0) {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            pgBase::AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            pgBase::SerializePtrs(layout, rsc, depth);
        }

        eType mResourceType;
        uint8_t mLayerCount;
        uint16_t mRefCount;
        int32_t field_C;
    };
    ASSERT_SIZE(grcTexture, 0x10);

    class grcTexturePC : public grcTexture
    {
    public:
        grcTexturePC() : grcTexture(), field_10(0), mName(nullptr), mD3DTexture(nullptr), mWidth(0), mHeight(0), mFormat(D3DFMT_UNKNOWN), 
                         mStride(0), mTextureType(eTextureType::STANDARD), mUnknown(0), mDynamic(0), mInitialized(false), mMipCount(0), 
                         field_28{0.0f, 0.0f, 0.0f}, field_34{0.0f, 0.0f, 0.0f}, mPrevious(nullptr), mNext(nullptr), mPixelData(nullptr), 
                         mIsSRGB(false), mCutMipLevels(0), field_4E(0), field_4F(0) {}

        grcTexturePC(const char* name) : grcTexture(), field_10(0), mName(nullptr), mD3DTexture(nullptr), mWidth(0), mHeight(0), mFormat(D3DFMT_UNKNOWN),
            mStride(0), mTextureType(eTextureType::STANDARD), mUnknown(0), mDynamic(0), mInitialized(false), mMipCount(0),
            field_28{0.0f, 0.0f, 0.0f}, field_34{0.0f, 0.0f, 0.0f}, mPrevious(nullptr), mNext(nullptr), mPixelData(nullptr),
            mIsSRGB(false), mCutMipLevels(0), field_4E(0), field_4F(0)
        {
            mName = strdup(name);
        };

        grcTexturePC(const datResource& rsc)
        {
            mPrevious = mNext = nullptr;

            rsc.PointerFixUp(mName);
            rsc.PointerFixUp(mPixelData);
        }

        grcTexturePC(const grcTexturePC& rhs)
        {
            memcpy(this, &rhs, sizeof(grcTexturePC));

            mPrevious = mNext = nullptr;
            mRefCount = 1;

            mName = nullptr;
            mName = strdup(rhs.mName);

            mPixelData = (void*)new uint8_t[rhs.GetSize()];
            memcpy(mPixelData, rhs.mPixelData, rhs.GetSize());
        }

        ~grcTexturePC()
        {
            if(mName)
            {
                delete[] mName;
                mName = nullptr;
            }

            if(mPixelData)
            {
                delete[] mPixelData;
                mPixelData = nullptr;
            }
        }

        grcTexturePC &operator=(const grcTexturePC& rhs) 
        {
            if(this == &rhs)
                return *this;

            memcpy(this, &rhs, sizeof(grcTexturePC));

            mPrevious = mNext = nullptr;
            mRefCount = 1;

            mName = nullptr;
            mName = _strdup(rhs.mName);

            mPixelData = (void*)new uint8_t[rhs.GetSize()];
            memcpy(mPixelData, rhs.mPixelData, rhs.GetSize());

            return *this;
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            grcTexture::AddToLayout(layout, depth);
            layout.AddObject(mName, RSC5Layout::eBlockType::VIRTUAL, strlen(mName) + 1);
            layout.AddObject((uint8_t*)mPixelData, RSC5Layout::eBlockType::PHYSICAL, GetSize());
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            grcTexture::SerializePtrs(layout, rsc, depth);
            
            layout.SerializePtr(mName, strlen(mName) + 1);
            layout.SerializePtr(mPixelData, GetSize());
        }

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) grcTexturePC(rsc);
        }

        uint32_t GetSize() const;
        DDS_HEADER GenerateDDSHeader() const;
        void LoadFromDDS(const DDS_HEADER& header, const void* pixelData);

        int32_t field_10;
        char* mName;
        struct IDirect3DTexture9* mD3DTexture;
        uint16_t mWidth;
        uint16_t mHeight;
        D3DFORMAT mFormat;
        uint16_t mStride;

        eTextureType mTextureType : 5;
        //locked?
        uint8_t mUnknown : 1;
        bool mDynamic : 1;
        bool mInitialized : 1;

        uint8_t mMipCount;
        float field_28[3];
        float field_34[3];
        grcTexturePC* mPrevious;
        grcTexturePC* mNext;
        void* mPixelData;
        bool mIsSRGB;
        uint8_t mCutMipLevels;
        uint8_t field_4E;
        uint8_t field_4F;
    };
    ASSERT_SIZE(grcTexturePC, 0x50);
}