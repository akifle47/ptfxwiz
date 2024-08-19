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
        grcTextureReference(const datResource& rsc)
        {
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
}