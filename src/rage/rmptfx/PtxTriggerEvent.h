#pragma once
#include "../Resource.h"
#include "../../Utils.h"

namespace rage
{
    class ptxTriggerEventScalars
    {
    public:
        ptxTriggerEventScalars();

        float field_0;
        float field_4;
        float field_8;
        int8_t field_C[4];
        float field_10;
        float field_14;
        uint32_t mColorTint;
        float mZoom;
        uint32_t mFlags;
    };
    ASSERT_SIZE(ptxTriggerEventScalars, 0x24);

    class ptxTriggerEvent
    {
    public:
        ptxTriggerEvent();

        ptxTriggerEvent(const datResource& rsc) : field_60(rsc) {}

        ptxTriggerEventScalars mEffectSpawnerAtRatio;
        int8_t field_24[0xC];
        ptxTriggerEventScalars mEffectSpawnerOnCollision;
        int8_t field_54[0xC];
        //todo: unknown type
        datOwner<void*> field_60;
        int32_t mType;
        float mTime;
        bool mToggle1;
        int8_t field_6D[0x2];
        int8_t field_6F;
    };
    ASSERT_SIZE(ptxTriggerEvent, 0x70);
}