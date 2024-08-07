#pragma once
#include "../../Utils.h"
#include "../ReferenceCounter.h"
#include "../DatOwner.h"
#include "RmPtfxShader.h"
#include "PtxRulePropList.h"
#include "PtxRenderState.h"
#include "PtxTriggerEvent.h"

namespace rage
{
    class ptxRule : public atReferenceCounter
    {
    public:
        ptxRule() = delete;

        ptxRule(const char* type);
        
        ptxRule(const datResource& rsc) : mSpawnEffectA(rsc), mSpawnEffectB(rsc)
        {
            rsc.PointerFixUp(mName);
            field_8 = 0;
            mFlags &= ~2;
        }

        void Place(void* that, const datResource& rsc);

        int32_t field_8;
        float mFileVersion;
        int32_t field_10;
        int8_t field_4[8];
        int8_t field_1C;
        ptxTriggerEvent mSpawnEffectA;
        ptxTriggerEvent mSpawnEffectB;
        ptxRenderState mRenderState;
        float mPhysicalRange;
        float mStopVelocity;
        uint32_t mFlags;
        int32_t field_120;
        char* mName;
        char mClassName[10];
        int8_t mPercentPhysical;
        int8_t mPercentKill;
        int8_t field_134[0x9];
    };
    ASSERT_SIZE(ptxRule, 0x140);


    class ptxSprite : public ptxRule
    {
    public:
        //ptxSprite();

        ptxSprite(const datResource& rsc) : ptxRule(rsc), field_184(rsc), mProps(rsc), mShader(rsc) {}

        float field_140;
        float field_144;
        float field_148;
        int8_t field_14C;
        float field_150;
        float field_154;
        float field_158;
        int8_t field_15C;
        Vector3 mAxisAlligned;
        int8_t field_16C;
        int32_t mStartTexFrameIndex;
        int32_t mEndTexFrameIndex;
        int32_t mEndTexAnimFrame;
        int32_t mNumTextureTilesX;
        int32_t mNumTextureTilesY;
        //todo: unknown type
        datOwner<void*> field_184;
        ptxSpriteRulePropList mProps;
        rmPtfxShader mShader;
        float mTrimCornersAmmount;
        bool mTrimCorners;
        int8_t field_69D[2];
        int8_t field_69F;
    };
    ASSERT_SIZE(ptxSprite, 0x6A0);


    class ptxModel : public ptxRule
    {
    public:
        ptxModel(const datResource& rsc) : ptxRule(rsc), mDrawables(rsc), mProps(rsc) {}

        Vector3 mRotation;
        int8_t field_14C[4];
        Vector3 mRotationVar;
        int8_t field_15C[4];
        Vector2 mRotationSpeedVar;
        atArray<PtxNameDrawablePair> mDrawables;
        void *field_170;
        ptxModelRulePropList mProps;
        int8_t field_698[7];
        int8_t field_69F;
    };
    ASSERT_SIZE(ptxModel, 0x6A0);
}