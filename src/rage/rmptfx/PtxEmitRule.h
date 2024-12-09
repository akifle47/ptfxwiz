#pragma once
#include "rapidjson/include/prettywriter.h"
#include "../../Utils.h"
#include "../ReferenceCounter.h"
#include "PtxDomain.h"
#include "../DatRef.h"

namespace rage
{
    class ptxEmitRuleStd : public atReferenceCounter
    {
    public:
        class stdEmitterData
        {
        public:
            stdEmitterData() : mEmitterDomain(nullptr), mVelocityDomain(nullptr), field_8{0}, field_10{0}, mPtxRuleName(nullptr) 
            {}

            stdEmitterData(const datResource& rsc) : mEmitterDomain(rsc), mVelocityDomain(rsc), mSpawnRateKF(rsc), mSpawnDistKF(rsc), 
                                                     mTimeScaleKF(rsc), mSpawnLifeKF(rsc), mSpeedKF(rsc), mSizeKFOT(rsc), mAccelerationKFOT(rsc), 
                                                     mDampeningKFOT(rsc), mMatrixWeightKFOT(rsc), mInheritVelKFOT(rsc) 
            {}
            
            virtual ~stdEmitterData()
            {
                if(mEmitterDomain.Get())
                {
                    delete mEmitterDomain.Get();
                    mEmitterDomain.Get() = nullptr;
                }

                if(mVelocityDomain.Get())
                {
                    delete mVelocityDomain.Get();
                    mVelocityDomain.Get() = nullptr;
                }
            };

            void AddToLayout(RSC5Layout& layout, uint32_t depth);
            void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

            void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
            void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object);

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
            //unused
            char* mPtxRuleName;
        private:
            ptxDomain* CreateDomain(uint32_t domainFunction, ptxDomain::eDomainType type);
        };
        ASSERT_SIZE(stdEmitterData, 0x1A8);

    public:
        ptxEmitRuleStd() : mDuration(1.0f), field_C(1), mEmitterData(), mKeyFrames{0}, mName(nullptr), mFileVersion(4.0f), field_1E8(0), mOneShot(false), field_1EC{0} {}

        ptxEmitRuleStd(const datResource& rsc) : mEmitterData(rsc), mKeyFrames{rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc, rsc} 
        {
            rsc.PointerFixUp(mName);
            field_1E8 = 0;

            assert(mFileVersion > 3.2f);
        }

        ~ptxEmitRuleStd()
        {
            if(mName)
            {
                delete[] mName;
                mName = nullptr;
            }
        };

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        void Place(void* that, const datResource& rsc)
        {
            new(that) ptxEmitRuleStd(rsc);
        }

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object);

        float mDuration;
        int32_t field_C;
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

    using ptxEmitRule = ptxEmitRuleStd;
}