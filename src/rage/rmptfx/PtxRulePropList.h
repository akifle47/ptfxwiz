#pragma once
#include "../Base.h"
#include "../Resource.h"
#include "PtxKeyFrame.h"

namespace rage
{
    class ptxRulePropList_obj1
    {
    public:
        ptxRulePropList_obj1();

        ptxRulePropList_obj1(const datResource& rsc) : field_40(rsc) {}

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) ptxRulePropList_obj1(rsc);
        }

        int8_t field_0[64];
        //todo: unknown type
        datOwner<void*> field_40;
        int8_t field_44[7];
        int8_t field_4B;
    };
    ASSERT_SIZE(ptxRulePropList_obj1, 0x4C);


    class ptxRulePropList_obj2
    {
    public:
        ptxRulePropList_obj2() {}

        ptxRulePropList_obj2(const datResource& rsc) : mKeyFrame(rsc), field_2C(rsc) {}

        rmPtfxKeyframe mKeyFrame;
        int8_t field_28[4];
        datOwner<void*> field_2C;
        int8_t field_30[3];
        int8_t field_33;
    };
    ASSERT_SIZE(ptxRulePropList_obj2, 0x34);


    class ptxRulePropList : public datBase
    {
    public:
        ptxRulePropList(const datResource& rsc) : field_4(rsc), field_10{rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc} {}

        atArray<ptxRulePropList_obj1> field_4;
        int8_t field_5[4];
        ptxRulePropList_obj2 field_10[15];
    };
    ASSERT_SIZE(ptxRulePropList, 0x31C);


    class ptxSpriteRulePropList : public ptxRulePropList
    {
    public:
        ptxSpriteRulePropList(const datResource& rsc) : ptxRulePropList(rsc), field_31C{rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc} {}

        ptxRulePropList_obj2 field_31C[9];
    };
    ASSERT_SIZE(ptxSpriteRulePropList, 0x4F0);


    class ptxModelRulePropList : public ptxRulePropList
    {
    public:
        ptxModelRulePropList(const datResource& rsc) : ptxRulePropList(rsc), field_31C{rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc} {}

        ptxRulePropList_obj2 field_31C[10];
    };
    ASSERT_SIZE(ptxModelRulePropList, 0x524);
}