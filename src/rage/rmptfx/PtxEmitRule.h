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

        ptxDomain* field_4;
        int8_t field_8[4];
        ptxDomain* field_C;
        int8_t field_10[4];
        rmPtfxKeyframe mKeyFrames[10];
        void* field_1A4;
    };
    ASSERT_SIZE(ptxEmitterData, 0x1A8);


    class ptxEmitRule : public atReferenceCounter
    {
        //idk
    };

    class ptxEmitRuleStd : public atReferenceCounter
    {
    public:
        class stdEmitterData : public ptxEmitterData {};

        int8_t field_8[8];
        ptxEmitRuleStd::stdEmitterData mEmitterData;
        void* field_1B8[10];
        char* field_1E0;
        float field_1E4;
        int field_1E8;
        int8_t field_1EC[4];
    };
    ASSERT_SIZE(ptxEmitRuleStd, 0x1F0);
}