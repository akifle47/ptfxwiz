#pragma once
#include "rapidjson/include/prettywriter.h"
#include "../Array.h"
#include "../DatOwner.h"
#include "PtxKeyFrame.h"

namespace rage
{
    class ptxEvolutionGroup
    {
    public:
        ptxEvolutionGroup() : field_10{}, mEvoListIndices(nullptr), field_28{}
        {}

        ptxEvolutionGroup(const datResource& rsc);

        ~ptxEvolutionGroup();

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) ptxEvolutionGroup(rsc); 
        }

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object);

        atArray<datOwner<class ptxEvolution>> mEvoList;
        atArray<struct ptxEvoBlendMode> mEvoBlendModeList;
        int8_t field_10[0x14];
        struct ptxEvoHashIndexList* mEvoListIndices;
        int8_t field_28[0x8];
    };
    ASSERT_SIZE(ptxEvolutionGroup, 0x30);


    class ptxEvolution
    {
    public:
        ptxEvolution();
        ptxEvolution(const datResource& rsc);

        ~ptxEvolution();

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) ptxEvolution(rsc);
        }

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object);

        char* mEvoName;
        atArray<datOwner<class ptxEvoPropList>> mPropList;
        int8_t field_C[0x8];
    };
    ASSERT_SIZE(ptxEvolution, 0x14);


    struct ptxEvoBlendMode
    {
        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            layout.AddObject(field_0, RSC5Layout::eBlockType::VIRTUAL, 40);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            layout.SerializePtr(field_0, 40);
        }

        inline void Place(void* that, const datResource& rsc)
        {
            rsc.PointerFixUp(field_0);
        }

        uint8_t* field_0;
        //count? but why are there 2? idk. always 40
        uint16_t field_4;
        uint16_t field_8;
    };
    ASSERT_SIZE(ptxEvoBlendMode, 0x8);

    class ptxEvoPropList
    {
    public:
        ptxEvoPropList() : field_0(-1), field_4(-1), field_8(-1), field_C(-1), field_10(-1), field_14(-1), field_18(-1), field_1C(-1), field_20(-1), field_24(-1)
        {};

        ptxEvoPropList(const datResource& rsc) : mPropList(rsc) 
        {}

        ~ptxEvoPropList();

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) ptxEvoPropList(rsc);
        }

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object);

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
        atArray<datOwner<class ptxEvoProp>> mPropList;
    };
    ASSERT_SIZE(ptxEvoPropList, 0x30);

    class ptxEvoProp
    {
    public:
        ptxEvoProp() : mRegID(-1) 
        {}

        ptxEvoProp(const datResource& rsc) : mKeyFrames(rsc) 
        {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) ptxEvoProp(rsc);
        }

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object);

        rmPtfxKeyframe mKeyFrames;
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

            if(mList.GetCount())
            {
                for(uint16_t i = 0; i < mList.GetCount(); i++)
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

            mList.Insert(newEntryIndex) = std::pair<uint32_t, uint32_t>(hash, index);
        }

        int8_t field_0;
        //first is a hash of ptxEvolutionGroup::mEvoList::mEvoName
        //second is an index into ptxEvolutionGroup::mEvoList
        atArray<std::pair<uint32_t, uint32_t>> mList;
    };
    ASSERT_SIZE(ptxEvoHashIndexList, 0xC);
};