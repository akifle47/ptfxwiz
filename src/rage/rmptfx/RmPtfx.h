#pragma once
#include "../../Utils.h"
#include "../Dictionary.h"
#include "../Texture.h"
#include "../Drawable.h"
#include "PtxRule.h"
#include "PtxEmitRule.h"
#include "PtxEffectRule.h"

#include <fstream>

namespace rage
{
    class RmPtfxLibrary
    {
    public:
        RmPtfxLibrary() {}

        RmPtfxLibrary(const RmPtfxLibrary& rhs)
        {
        }

        RmPtfxLibrary(const datResource& rsc)
        {
            mTextures.Place(&mTextures, rsc);
            //mDrawables.Place(&mDrawables, rsc);
            mRules.Place(&mRules, rsc);
            mEmitRules.Place(&mEmitRules, rsc);
            mEffectRules.Place(&mEffectRules, rsc);
        }

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) RmPtfxLibrary(rsc);
        }

        datOwner<pgDictionary<grcTexturePC>> mTextures;
        int8_t field_4[4];
        datOwner<pgDictionary<rmcDrawable>> mDrawables;
        datOwner<pgDictionary<ptxRule>> mRules;
        int32_t field_10;
        datOwner<pgDictionary<ptxEmitRule>> mEmitRules;
        datOwner<pgDictionary<ptxEffectRule>> mEffectRules;
    };
    ASSERT_SIZE(RmPtfxLibrary, 0x1C);
}