#pragma once
#include "../../Utils.h"
#include "../Base.h"
#include "../Array.h"
#include "../Vector.h"

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


    struct ptxKeyFrameEntry
    {
        float mTime;

        int8_t field_4[0xC];

        Vector4 mValue;
        Vector4 Delta;
    };
    ASSERT_SIZE(ptxKeyFrameEntry, 0x30);


    class rmPtfxKeyframe : public ptxNetObject
    {
    public:
        rmPtfxKeyframe() {}

        rmPtfxKeyframe(const datResource& rsc) : mEntries(rsc) {}

        int8_t field_15;
        atArray<ptxKeyFrameEntry> mEntries;
        int32_t field_20;
        int32_t field_24;
    };
    ASSERT_SIZE(rmPtfxKeyframe, 0x28);
}