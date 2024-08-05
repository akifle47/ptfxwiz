#pragma once
#include "../../Utils.h"
#include "../ReferenceCounter.h"
#include "PtxKeyFrame.h"
#include "PtxTimeline.h"

namespace rage
{
    struct ptxEffectRule_obj1
    {
        atArray<struct ptxEffectRule_obj1_obj1> field_0;
        //todo: unknown 8 bytes
        atArray<uint64_t> field_8;
        int8_t field_10[0x14];
        struct ptxEffectRule_obj2* field_24;
    };
    ASSERT_SIZE(ptxEffectRule_obj1, 0x28);


    struct ptxEffectRule_obj1_obj1
    {
        char* field_0;
        atArray<struct ptxEffectRule_obj1_obj1_obj1> field_4;
    };
    ASSERT_SIZE(ptxEffectRule_obj1_obj1, 0xC);

    struct ptxEffectRule_obj1_obj1_obj1
    {
        void* field_0;
        int8_t field_4[0x24];
        atArray<rmPtfxKeyframe> mKeyframes;
    };
    ASSERT_SIZE(ptxEffectRule_obj1_obj1_obj1, 0x30);


    struct ptxEffectRule_obj2
    {
        int8_t field_0;
        //first is (probably) a hash of ptxEffectRule_obj1::field_0::field_0
        //second is an index into ptxEffectRule_obj1::field_0
        atArray<std::pair<uint32_t, uint32_t>> field_4;
    };
    ASSERT_SIZE(ptxEffectRule_obj2, 0xC);


    class ptxEffectRule : public atReferenceCounter
    {
    public:
        rmPtfxKeyframe mKeyFrames[6];
        void* field_F8;
        int32_t field_FC;
        ptxEffectRule_obj1* field_100;
        float field_104;
        void* field_108;
        int8_t field_10C;
        int8_t field_10D;
        int8_t field_10E;
        int8_t field_10F[2];
        int8_t field_111;
        int8_t field_112;
        int8_t field_113;
        int32_t field_114;
        int8_t field_118[7];
        int8_t field_11F;
    };
    ASSERT_SIZE(ptxEffectRule, 0x120);


    class ptxEffectRuleStd : public ptxEffectRule
    {
    public:
        ptxTimeLine mTimeline;
        float field_144;
        float field_148;
        float field_14C;
        float field_150;
        float field_154;
        float field_158;
        float field_15C;
        float field_160;
        float field_164;
        float field_168;
        int32_t field_16C;
        int32_t field_170;
        int8_t field_174;
        int8_t field_175;
        int8_t field_176;
        int8_t field_177;
        int8_t field_178;
        int8_t field_179;
        int8_t field_17A[4];
        float field_180;
        float field_184;
        float field_188;
        int8_t field_18C[4];
        float field_190;
        float field_194;
        float field_198;
        int8_t field_19C[3];
        int8_t field_19F;
    };
    ASSERT_SIZE(ptxEffectRuleStd, 0x1A0);
}