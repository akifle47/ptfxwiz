#pragma once
#include "rapidjson/include/prettywriter.h"
#include "../../Utils.h"
#include "../ReferenceCounter.h"
#include "PtxDomain.h"
#include "../DatRef.h"

namespace rage
{
    class ptxEmitterData
    {
    public:
        virtual ~ptxEmitterData() = default;
    };

    class ptxEmitRule : public atReferenceCounter
    {
    public:
        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        void Place(void* that, const datResource& rsc);

        virtual void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) = 0;

        uint32_t GetObjectSize() const
        {
            return 0x1F0;
        }
    };

    class ptxEmitRuleStd : public ptxEmitRule
    {
    public:
        class stdEmitterData : public ptxEmitterData
        {
        public:
            stdEmitterData(const datResource& rsc) : mEmitterDomain(rsc), mVelocityDomain(rsc), mSpawnRateKF(rsc), mSpawnDistKF(rsc), 
                                                     mTimeScaleKF(rsc), mSpawnLifeKF(rsc), mSpeedKF(rsc), mSizeKFOT(rsc), mAccelerationKFOT(rsc), 
                                                     mDampeningKFOT(rsc), mMatrixWeightKFOT(rsc), mInheritVelKFOT(rsc)
            {
                if(mPtxRuleName)
                    rsc.PointerFixUp(mPtxRuleName);
            }
            
            void AddToLayout(RSC5Layout& layout, uint32_t depth);
            void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

            void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

            datOwner<ptxDomain> mEmitterDomain;
            int8_t field_8[4];
            datOwner<ptxDomain> mVelocityDomain;
            int8_t field_10[4];
            rmPtfxKeyframe mSpawnRateKF;
            rmPtfxKeyframe mSpawnDistKF;
            rmPtfxKeyframe mTimeScaleKF;
            rmPtfxKeyframe mSpawnLifeKF;
            rmPtfxKeyframe mSpeedKF;
            rmPtfxKeyframe mSizeKFOT;
            rmPtfxKeyframe mAccelerationKFOT;
            rmPtfxKeyframe mDampeningKFOT;
            rmPtfxKeyframe mMatrixWeightKFOT;
            rmPtfxKeyframe mInheritVelKFOT;
            //seems to always be null
            char* mPtxRuleName;
        };
        ASSERT_SIZE(stdEmitterData, 0x1A8);

    public:
        ptxEmitRuleStd(const datResource& rsc) : mEmitterData(rsc), mKeyFrames{rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc} 
        {
            rsc.PointerFixUp(mName);
            field_1E8 = 0;

            assert(mFileVersion > 3.2f);
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;

        float mDuration;
        int8_t field_C[4];
        stdEmitterData mEmitterData;
        //dont think this is used.
        datRef<rmPtfxKeyframe> mKeyFrames[10];
        char* mName;
        float mFileVersion;
        int32_t field_1E8;
        bool mOneShot;
        int8_t field_1EC[3];
    };
    ASSERT_SIZE(ptxEmitRuleStd, 0x1F0);
}