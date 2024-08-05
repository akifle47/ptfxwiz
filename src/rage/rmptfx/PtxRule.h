#pragma once
#include "../../Utils.h"
#include "../ReferenceCounter.h"
#include "../DatOwner.h"
#include "RmPtfxShader.h"
#include "PtxRulePropList.h"

namespace rage
{
    class ptxRule_obj1
    {
    public:
        ptxRule_obj1();

        ptxRule_obj1(const datResource& rsc) : field_60(rsc) {}

        float field_0;
        float field_4;
        float field_8;
        int8_t field_C[0x4];
        float field_10;
        float field_14;
        int32_t field_18;
        float field_1C;
        int32_t field_20;
        int8_t field_24[0xC];
        float field_30;
        float field_34;
        float field_38;
        int8_t field_3C[0x4];
        float field_40;
        float field_44;
        int32_t field_48;
        float field_4C;
        //flags?
        int32_t field_50;
        int8_t field_54[0xC];
        //todo: unknown type
        datOwner<void*> field_60;
        int32_t field_64;
        float field_68;
        int8_t field_6C;
        int8_t field_6D[0x2];
        int8_t field_6F;
    };
    ASSERT_SIZE(ptxRule_obj1, 0x70);


    class ptxRule : public atReferenceCounter
    {
    public:
        ptxRule() = delete;

        ptxRule(const char* type);
        
        ptxRule(const datResource& rsc) : field_20(rsc), field_90(rsc), field_124(rsc)
        {
            field_8 = 0;
            field_11C &= ~2;
        }

        void Place(void* that, const datResource& rsc);

        int32_t field_8;
        float field_C;
        int32_t field_10;
        int8_t field_4[8];
        int8_t field_1C;
        ptxRule_obj1 field_20;
        ptxRule_obj1 field_90;
        int32_t field_100;
        int32_t field_104;
        float field_108;
        int32_t field_10C;
        int8_t field_110;
        int8_t field_111;
        int8_t field_112;
        float field_114;
        float field_118;
        //flags?
        int32_t field_11C;
        int32_t field_120;
        //todo: unknown type
        datOwner<void*> field_124;
        char mType[10];
        int8_t field_132;
        int8_t field_133;
        int8_t field_134[0x9];
    };
    ASSERT_SIZE(ptxRule, 0x140);


    class ptxSprite : public ptxRule
    {
    public:
        //ptxSprite();

        ptxSprite(const datResource& rsc) : ptxRule(rsc), field_184(rsc), mPropList(rsc), mShader(rsc) {}

        float field_140;
        float field_144;
        float field_148;
        int8_t field_14C;
        float field_150;
        float field_154;
        float field_158;
        int8_t field_15C;
        float field_160;
        float field_164;
        float field_168;
        int8_t field_16C;
        int32_t field_170;
        int32_t field_174;
        int32_t field_178;
        int32_t field_17C;
        int32_t field_180;
        //todo: unknown type
        datOwner<void*> field_184;
        ptxSpriteRulePropList mPropList;
        rmPtfxShader mShader;
        float field_698;
        int8_t field_69C;
        int8_t field_69D[2];
        int8_t field_69F;
    };
    ASSERT_SIZE(ptxSprite, 0x6A0);


    //ASSERT_SIZE(ptxModel, 0x6A0);
}