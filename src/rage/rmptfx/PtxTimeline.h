#pragma once
#include "../../Utils.h"
#include "../Array.h"
#include "../DatRef.h"
#include "PtxEvent.h"

namespace rage
{
    class ptxTimeLine
    {
    public:
        ptxTimeLine() : field_4(1.0f), mPreUpdate(0.0f), mNumLoops(0), mEvents(), mEffectRule(nullptr), field_1C{}
        {}

        ptxTimeLine(const datResource& rsc);

        virtual ~ptxTimeLine();

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object);

        float field_4;
        float mPreUpdate;
        int32_t mNumLoops;
        atArray<datOwner<ptxEvent>> mEvents;
        datRef<class ptxEffectRuleStd> mEffectRule;
        int8_t field_1C[8];
    };
    ASSERT_SIZE(ptxTimeLine, 0x24);
}