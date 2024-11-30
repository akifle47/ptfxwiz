#include "PtxEmitRule.h"

namespace rage
{
    void ptxEmitRule::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        ((ptxEmitRuleStd*)this)->AddToLayout(layout, depth);
    }

    void ptxEmitRule::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        ((ptxEmitRuleStd*)this)->SerializePtrs(layout, rsc, depth);
    }

    void ptxEmitRule::Place(void* that, const datResource& rsc)
    {
        new(that) ptxEmitRuleStd(rsc);
    }

    void ptxEmitRuleStd::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mEmitterData.AddToLayout(layout, depth);

        for(size_t i = 0; i < std::size(mKeyFrames); i++)
        {
            mKeyFrames[i].AddToLayout(layout, depth);
        }

        if(mName)
            layout.AddObject(mName, RSC5Layout::eBlockType::VIRTUAL, strlen(mName) + 1);
    }

    void ptxEmitRuleStd::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mEmitterData.SerializePtrs(layout, rsc, depth);

        for(size_t i = 0; i < std::size(mKeyFrames); i++)
        {
            mKeyFrames[i].SerializePtrs(layout, rsc, depth);
        }

        if(mName)
            layout.SerializePtr(mName, strlen(mName) + 1);
    }

    void ptxEmitRuleStd::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            writer.String("Name");
            writer.String(mName);

            writer.String("Version");
            writer.Uint(100);

            writer.String("Duration");
            writer.Double((double)mDuration);

            writer.String("EmitterData");
            mEmitterData.WriteToJson(writer);

            writer.String("field_1E8");
            writer.Int(field_1E8);

            writer.String("OneShot");
            writer.Bool(mOneShot);
        }
        writer.EndObject();
    }

    void ptxEmitRuleStd::stdEmitterData::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mEmitterDomain.AddToLayout(layout, depth);
        mVelocityDomain.AddToLayout(layout, depth);

        mSpawnRateKF.AddToLayout(layout, depth);
        mSpawnDistKF.AddToLayout(layout, depth);
        mTimeScaleKF.AddToLayout(layout, depth);
        mSpawnLifeKF.AddToLayout(layout, depth);
        mSpeedKF.AddToLayout(layout, depth);
        mSizeKFOT.AddToLayout(layout, depth);
        mAccelerationKFOT.AddToLayout(layout, depth);
        mDampeningKFOT.AddToLayout(layout, depth);
        mMatrixWeightKFOT.AddToLayout(layout, depth);
        mInheritVelKFOT.AddToLayout(layout, depth);
        
        if(mPtxRuleName)
            layout.AddObject(mPtxRuleName, RSC5Layout::eBlockType::VIRTUAL, strlen(mPtxRuleName) + 1);
    }

    void ptxEmitRuleStd::stdEmitterData::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mEmitterDomain.SerializePtrs(layout, rsc, depth);
        mVelocityDomain.SerializePtrs(layout, rsc, depth);

        mSpawnRateKF.SerializePtrs(layout, rsc, depth);
        mSpawnDistKF.SerializePtrs(layout, rsc, depth);
        mTimeScaleKF.SerializePtrs(layout, rsc, depth);
        mSpawnLifeKF.SerializePtrs(layout, rsc, depth);
        mSpeedKF.SerializePtrs(layout, rsc, depth);
        mSizeKFOT.SerializePtrs(layout, rsc, depth);
        mAccelerationKFOT.SerializePtrs(layout, rsc, depth);
        mDampeningKFOT.SerializePtrs(layout, rsc, depth);
        mMatrixWeightKFOT.SerializePtrs(layout, rsc, depth);
        mInheritVelKFOT.SerializePtrs(layout, rsc, depth);

        if(mPtxRuleName)
            layout.SerializePtr(mPtxRuleName, strlen(mPtxRuleName) + 1);
    }

    void ptxEmitRuleStd::stdEmitterData::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            if(mEmitterDomain.Get())
            {
                writer.String("EmitterDomain");
                mEmitterDomain->WriteToJson(writer);
            }

            if(mVelocityDomain.Get())
            {
                writer.String("VelocityDomain");
                mVelocityDomain->WriteToJson(writer);
            }

            writer.String("SpawnRateKF");
            mSpawnRateKF.WriteToJson(writer);
            writer.String("SpawnDistKF");
            mSpawnDistKF.WriteToJson(writer);
            writer.String("TimeScaleKF");
            mTimeScaleKF.WriteToJson(writer);
            writer.String("SpawnLifeKF");
            mSpawnLifeKF.WriteToJson(writer);
            writer.String("SpeedKF");
            mSpeedKF.WriteToJson(writer);
            writer.String("SizeKFOT");
            mSizeKFOT.WriteToJson(writer);
            writer.String("AccelerationKFOT");
            mAccelerationKFOT.WriteToJson(writer);
            writer.String("DampeningKFOT");
            mDampeningKFOT.WriteToJson(writer);
            writer.String("MatrixWeightKFOT");
            mMatrixWeightKFOT.WriteToJson(writer);
            writer.String("InheritVelKFOT");
            mInheritVelKFOT.WriteToJson(writer);

            if(mPtxRuleName)
            {
                writer.String("PtxRuleName");
                writer.String(mPtxRuleName);
            }
        }
        writer.EndObject();
    }
}