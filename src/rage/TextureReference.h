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

            //todo ?
        }

        uint32_t field_10;
        char* mName;
        grcTexture* mTexture;
    };
}