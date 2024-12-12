#pragma once
#include "../../Utils.h"
#include "../Dictionary.h"
#include "../grcore/Texture.h"
#include "../grmodel/Drawable.h"
#include "PtxRule.h"
#include "PtxEmitRule.h"
#include "PtxEffectRule.h"

namespace rage
{
    class PtxList
    {
    public:
        PtxList() : mTextures(nullptr), mDrawables(nullptr), mRules(nullptr), mEmitRules(nullptr), mEffectRules(nullptr), field_4{0, 0, 0, 0}, field_10(0) {}
        
        PtxList(const datResource& rsc) : field_4{0, 0, 0, 0}, field_10(0)
        {
            mTextures.Place(&mTextures, rsc);
            mDrawables.Place(&mDrawables, rsc);
            mRules.Place(&mRules, rsc);
            mEmitRules.Place(&mEmitRules, rsc);
            mEffectRules.Place(&mEffectRules, rsc);
        }

        ~PtxList()
        {
            if(mTextures.Get())
            {
                delete mTextures.Get();
                mTextures = {0};
            }
            if(mDrawables.Get())
            {
                delete mDrawables.Get();
                mDrawables = {0};
            }
            if(mRules.Get())
            {
                delete mRules.Get();
                mRules = {0};
            }
            if(mEmitRules.Get())
            {
                delete mEmitRules.Get();
                mEmitRules = {0};
            }
            if(mEffectRules.Get())
            {
                delete mEffectRules.Get();
                mEffectRules = {0};
            }
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            Log::Info("Saving textures...");
            mTextures.AddToLayout(layout, depth);

            Log::Info("Saving drawables...");
            mDrawables.AddToLayout(layout, depth);

            Log::Info("Saving rules...");
            mRules.AddToLayout(layout, depth);

            Log::Info("Saving emit rules...");
            mEmitRules.AddToLayout(layout, depth);

            Log::Info("Saving effect rules...");
            mEffectRules.AddToLayout(layout, depth);

            Log::Info("Saving resource...");
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
        bool LoadFromJson(const std::filesystem::path& filePath);

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

    private:
        void LoadTextures(std::filesystem::path& texturesDir);
        void LoadDrawables(std::filesystem::path& drawablesDir);

    };
    ASSERT_SIZE(PtxList, 0x1C);
}