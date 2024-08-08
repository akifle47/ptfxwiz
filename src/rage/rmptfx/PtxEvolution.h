#pragma once
#include "../Array.h"
#include "PtxKeyFrame.h"

namespace rage
{
    class ptxEvolutionGroup
    {
    public:
        ptxEvolutionGroup(const datResource& rsc);

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) ptxEvolutionGroup(rsc); 
        }

        atArray<class ptxEvolution> mEvoList;
        atArray<struct ptxEvoBlendMode> mEvoBlendModeList;
        int8_t field_10[0x14];
        struct ptxEvoHashIndexList* mEvoListIndices;
        int8_t field_28[0x8];
    };
    ASSERT_SIZE(ptxEvolutionGroup, 0x30);


    class ptxEvolution
    {
    public:
        ptxEvolution(const datResource& rsc) : mPropList(rsc)
        {
            if(mEvoName)
            {
                rsc.PointerFixUp(mEvoName);
            }
        }

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) ptxEvolution(rsc);
        }

        char* mEvoName;
        atArray<class ptxEvoPropList> mPropList;
        int8_t field_C[0x8];
    };
    ASSERT_SIZE(ptxEvolution, 0x14);


    struct ptxEvoBlendMode
    {
        int32_t field_0;
        int32_t field_4;
    };
    ASSERT_SIZE(ptxEvoBlendMode, 0x8);

    class ptxEvoPropList
    {
    public:
        ptxEvoPropList(const datResource& rsc) : mPropList(rsc) {}

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) ptxEvoPropList(rsc);
        }

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
        atArray<class ptxEvoProp> mPropList;
    };
    ASSERT_SIZE(ptxEvoPropList, 0x30);

    class ptxEvoProp
    {
    public:
        ptxEvoProp(const datResource& rsc) : mKeyFrameData(rsc) {}

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) ptxEvoProp(rsc);
        }

        rmPtfxKeyframe mKeyFrameData;
        int32_t mRegID;
    };
    ASSERT_SIZE(ptxEvoProp, 0x2C);


    struct ptxEvoHashIndexList
    {
        ptxEvoHashIndexList(uint16_t count) : mList(count)
        {
            field_0 = 1;
        }

        void AddToList(const uint32_t hash, const uint32_t index)
        {
            uint16_t newEntryIndex = 0;

            if(mList.mCount)
            {
                for(uint16_t i = 0; i < mList.mCount; i++)
                {
                    if(mList[i].first < hash)
                        newEntryIndex = i;

                    if(mList[i].first == hash)
                    {
                        mList[i].second = index;
                        return;
                    }
                }
            }

            mList.Insert(*new std::pair<uint32_t, uint32_t>(hash, index), newEntryIndex);
        }

        int8_t field_0;
        //first is a hash of ptxEvolutionGroup::mEvoList::mEvoName
        //second is an index into ptxEvolutionGroup::mEvoList
        atArray<std::pair<uint32_t, uint32_t>> mList;
    };
    ASSERT_SIZE(ptxEvoHashIndexList, 0xC);
};