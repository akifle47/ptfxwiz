#pragma once
#include "../../Utils.h"
#include "../Base.h"
#include "../Array.h"

namespace rage
{
    class ptxNetObject : public datBase
    {
    public:
        int32_t field_4;
        int32_t field_8;
        int32_t field_C;
        int32_t field_10;
    };
    ASSERT_SIZE(ptxNetObject, 0x14);


    struct rmPtfxKeyframe_obj1
    {
        float field_0;
        float field_4;
        float field_8;
        float field_C;
        float field_10;
        float field_14;
        float field_18;
        float field_1C;
        float field_20;
        float field_24;
        float field_28;
        float field_2C;
    };
    ASSERT_SIZE(rmPtfxKeyframe_obj1, 0x30);


    class rmPtfxKeyframe : ptxNetObject
    {
    public:
        rmPtfxKeyframe() {}

        rmPtfxKeyframe(const datResource& rsc) : field_18(rsc) {}

        int8_t field_15;
        atArray<rmPtfxKeyframe_obj1> field_18;
        int32_t field_20;
        int32_t field_24;
    };
    ASSERT_SIZE(rmPtfxKeyframe, 0x28);
}