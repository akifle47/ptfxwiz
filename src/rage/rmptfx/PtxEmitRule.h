#pragma once
#include "../../Utils.h"
#include "../ReferenceCounter.h"
#include "PtxDomain.h"

namespace rage
{
    class ptxEmitterData
    {
    public:
        virtual ~ptxEmitterData() = default;
    };

    class ptxEmitRule : public atReferenceCounter
    {
    public:
        void Place(void* that, const datResource& rsc);
    };

    class ptxEmitRuleStd : public ptxEmitRule
    {
    public:
        class stdEmitterData : public ptxEmitterData
        {
        public:
            stdEmitterData(const datResource& rsc) : field_4(rsc), field_C(rsc), mKeyFrames{rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc}, field_1A4(rsc) {}
            
            datOwner<ptxDomain> field_4;
            int8_t field_8[4];
            datOwner<ptxDomain> field_C;
            int8_t field_10[4];
            rmPtfxKeyframe mKeyFrames[10];
            //seems to always be null
            datOwner<void*> field_1A4;
        };
        ASSERT_SIZE(stdEmitterData, 0x1A8);

    public:
        ptxEmitRuleStd(const datResource& rsc) : mEmitterData(rsc), mKeyFrames{rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc} 
        {
            rsc.PointerFixUp(mName);
            field_1E8 = 0;

            assert(field_1E4 > 3.2f);
        }

        float field_8;
        int8_t field_C[4];
        stdEmitterData mEmitterData;
        datOwner<rmPtfxKeyframe> mKeyFrames[10];
        char* mName;
        float field_1E4;
        int32_t field_1E8;
        int8_t field_1EC[4];
    };
    ASSERT_SIZE(ptxEmitRuleStd, 0x1F0);
}