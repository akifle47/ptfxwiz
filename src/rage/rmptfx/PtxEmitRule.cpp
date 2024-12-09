#include "PtxEmitRule.h"
#include "JsonHelpers.h"

namespace rage
{
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

    void ptxEmitRuleStd::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        mName = strdup(object["Name"].GetString());
        mDuration = object["Duration"].GetFloat();

        JsonHelpers::LoadMemberObject(mEmitterData, object, "EmitterData");

        field_1E8 = object["field_1E8"].GetInt();
        mOneShot = object["OneShot"].GetBool();
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
        }
        writer.EndObject();
    }

    void ptxEmitRuleStd::stdEmitterData::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        if(object.HasMember("EmitterDomain") && object["EmitterDomain"].IsObject())
        {
            auto domainObject = object["EmitterDomain"].GetObject();
            ptxDomain::eDomainType type = ptxDomain::StringToType(domainObject["Type"].GetString());
            mEmitterDomain = CreateDomain(0, type);
            mEmitterDomain->LoadFromJson(domainObject);
        }

        if(object.HasMember("VelocityDomain") && object["VelocityDomain"].IsObject())
        {
            auto domainObject = object["VelocityDomain"].GetObject();
            ptxDomain::eDomainType type = ptxDomain::StringToType(domainObject["Type"].GetString());
            mVelocityDomain = CreateDomain(1, type);
            mVelocityDomain->LoadFromJson(domainObject);
        }

        JsonHelpers::LoadMemberObject(mSpawnRateKF, object, "SpawnRateKF");

        JsonHelpers::LoadMemberObject(mSpawnDistKF, object, "SpawnDistKF");
        JsonHelpers::LoadMemberObject(mTimeScaleKF, object, "TimeScaleKF");
        JsonHelpers::LoadMemberObject(mSpawnLifeKF, object, "SpawnLifeKF");
        JsonHelpers::LoadMemberObject(mSpeedKF, object, "SpeedKF");
        JsonHelpers::LoadMemberObject(mSizeKFOT, object, "SizeKFOT");
        JsonHelpers::LoadMemberObject(mAccelerationKFOT, object, "AccelerationKFOT");
        JsonHelpers::LoadMemberObject(mDampeningKFOT, object, "DampeningKFOT");
        JsonHelpers::LoadMemberObject(mMatrixWeightKFOT, object, "MatrixWeightKFOT");
        JsonHelpers::LoadMemberObject(mInheritVelKFOT, object, "InheritVelKFOT");
    }

    ptxDomain* ptxEmitRuleStd::stdEmitterData::CreateDomain(uint32_t domainFunction, ptxDomain::eDomainType type)
    {
        switch(type)
        {
            case ptxDomain::eDomainType::BOX:
                return new ptxDomainBox(domainFunction);
            break;

            case ptxDomain::eDomainType::SPHERE:
                return new ptxDomainSphere(domainFunction);
            break;

            case ptxDomain::eDomainType::CYLINDER:
                return new ptxDomainCylinder(domainFunction);
            break;

            case ptxDomain::eDomainType::VORTEX:
                return new ptxDomainVortex(domainFunction);
            break;

            default:
                Log::Error("Invalid ptx domain type - %d", type);
               return nullptr;
        }
    }
}