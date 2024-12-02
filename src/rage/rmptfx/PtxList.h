#pragma once
#include "../../Utils.h"
#include "../Dictionary.h"
#include "../grcore/Texture.h"
#include "../grmodel/Drawable.h"
#include "PtxRule.h"
#include "PtxEmitRule.h"
#include "PtxEffectRule.h"

#include <fstream>

namespace rage
{
    class PtxList
    {
    public:
        PtxList() {}
        
        PtxList(const datResource& rsc)
        {
            mTextures.Place(&mTextures, rsc);
            mDrawables.Place(&mDrawables, rsc);
            mRules.Place(&mRules, rsc);
            mEmitRules.Place(&mEmitRules, rsc);
            mEffectRules.Place(&mEffectRules, rsc);
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mTextures.AddToLayout(layout, depth);
            mDrawables.AddToLayout(layout, depth);
            mRules.AddToLayout(layout, depth);
            mEmitRules.AddToLayout(layout, depth);
            mEffectRules.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mTextures.SerializePtrs(layout, rsc, depth);
            mDrawables.SerializePtrs(layout, rsc, depth);
            mRules.SerializePtrs(layout, rsc, depth);
            mEmitRules.SerializePtrs(layout, rsc, depth);
            mEffectRules.SerializePtrs(layout, rsc, depth);
        }

        void SaveToJson(const std::filesystem::path& filePath);

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) PtxList(rsc);
        }
        
        datOwner<pgDictionary<grcTexturePC>> mTextures;
        int8_t field_4[4];
        datOwner<pgDictionary<rmcDrawable>> mDrawables;
        datOwner<pgDictionary<ptxRule>> mRules;
        int32_t field_10;
        datOwner<pgDictionary<ptxEmitRule>> mEmitRules;
        datOwner<pgDictionary<ptxEffectRule>> mEffectRules;
    };
    ASSERT_SIZE(PtxList, 0x1C);
}