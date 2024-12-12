#pragma once
#include "Texture.h"

namespace rage
{
    class grcTextureReferenceBase : public grcTexture
    {
    };

    class grcTextureReference : public grcTextureReferenceBase
    {
    public:
        grcTextureReference(const char* name)
        {
            mResourceType = eType::REFERENCE;
            field_10 = 0;
            mName = strdup(name);
            mTexture = nullptr;
        }

        ~grcTextureReference()
        {
            if(mName)
                delete[] mName;
        }

        grcTextureReference(const datResource& rsc)
        {
            mResourceType = eType::REFERENCE;
            field_10 = 0;
            rsc.PointerFixUp(mName);
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            grcTexture::AddToLayout(layout, depth);
            layout.AddObject(mName, RSC5Layout::eBlockType::VIRTUAL, strlen(mName) + 1);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            grcTexture::SerializePtrs(layout, rsc, depth);
            layout.SerializePtr(mName, strlen(mName) + 1);
        }

        uint32_t field_10;
        char* mName;
        grcTexture* mTexture;
    };
    ASSERT_SIZE(grcTextureReference, 0x1C);
}