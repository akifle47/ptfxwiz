#pragma once
#include "../Array.h"
#include "PtxKeyFrame.h"

namespace rage
{
    class ptxEvolutionGroup
    {
    public:
        atArray<struct ptxEvolution> mEvoList;
        atArray<struct ptxEvoBlendMode> mEvoBlendModeList;
        int8_t field_10[0x14];
        struct ptxEvoHashIndexList* mEvoListIndices;
        int8_t field_28[0x8];
    };
    ASSERT_SIZE(ptxEvolutionGroup, 0x30);


    struct ptxEvolution
    {
        char* mEvoName;
        atArray<struct ptxEvoPropList> mPropList;
        int8_t field_C[0x8];
    };
    ASSERT_SIZE(ptxEvolution, 0x14);

    struct ptxEvoBlendMode
    {
        int32_t field_0;
        int32_t field_4;
    };
    ASSERT_SIZE(ptxEvoBlendMode, 0x8);

    struct ptxEvoPropList
    {
        int32_t field_0;
        int32_t field_4;
        int32_t field_8;
        int32_t field_C;
        int32_t field_10;
        int32_t field_14;
        int32_t field_18;
        int32_t field_1C;
        int32_t field_20;
        int32_t field_24;
        atArray<struct ptxEvoProp> mPropList;
    };
    ASSERT_SIZE(ptxEvoPropList, 0x30);

    struct ptxEvoProp
    {
        rmPtfxKeyframe mKeyFrameData;
        int32_t mRegID;
    };
    ASSERT_SIZE(ptxEvoProp, 0x2C);

    struct ptxEvoHashIndexList
    {
        int8_t field_0;
        //first is a hash of ptxEvolutionGroup::mEvoList::mEvoName
        //second is an index into ptxEvolutionGroup::mEvoList
        atArray<std::pair<uint32_t, uint32_t>> mList;
    };
    ASSERT_SIZE(ptxEvoHashIndexList, 0xC);
};