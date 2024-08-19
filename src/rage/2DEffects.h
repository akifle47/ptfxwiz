#pragma once
#include "math/Vector.h"
#include "../Utils.h"

class C2dEffect
{
public:
    virtual ~C2dEffect() = default;

    rage::Vector3 mPosition;
};
ASSERT_SIZE(C2dEffect, 0x10);


class CLightAttr : public C2dEffect
{
public:
    rage::Vector3 mDirection;
    rage::Vector3 mOrigin;
    uint8_t mColor[3];
    uint8_t mFlashiness;
    float field_2C;
    float field_30;
    float field_34;
    float mDistance;
    float mRange;
    float mCoronaSize;
    float mHotSpot;
    float mFalloff;
    uint32_t mFlags;
    uint32_t mCoronaHash;
    float mIntensityMaskHash;
    uint32_t mFlags2 : 24;
    uint32_t mType   : 8;
    float field_5C;
    float field_60;
    float field_64;
    int16_t mBoneID;
    int16_t field_6A;
};
ASSERT_SIZE(CLightAttr, 0x6C);