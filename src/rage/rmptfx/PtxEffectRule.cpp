#include "PtxEffectRule.h"
#include "JsonHelpers.h"

namespace rage
{
    void ptxEffectRule::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mKFColorTint.AddToLayout(layout, depth);
        mKFColorTintMax.AddToLayout(layout, depth);
        mKFZoom.AddToLayout(layout, depth);
        mKFRotation.AddToLayout(layout, depth);
        mKFDataSphere.AddToLayout(layout, depth);
        mKFDataCapsuleA.AddToLayout(layout, depth);

        if(mName)
            layout.AddObject(mName, RSC5Layout::eBlockType::VIRTUAL, strlen(mName) + 1);

        mPtxEvoGroup.AddToLayout(layout, depth);

        ((ptxEffectRuleStd*)this)->AddToLayout(layout, depth);
    }

    void ptxEffectRule::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mKFColorTint.SerializePtrs(layout, rsc,  depth);
        mKFColorTintMax.SerializePtrs(layout, rsc,  depth);
        mKFZoom.SerializePtrs(layout, rsc,  depth);
        mKFRotation.SerializePtrs(layout, rsc,  depth);
        mKFDataSphere.SerializePtrs(layout, rsc,  depth);
        mKFDataCapsuleA.SerializePtrs(layout, rsc,  depth);

        if(mName)
            layout.SerializePtr(mName, strlen(mName) + 1);

        mPtxEvoGroup.SerializePtrs(layout, rsc,  depth);

        ((ptxEffectRuleStd*)this)->SerializePtrs(layout, rsc, depth);
    }

    void ptxEffectRule::Place(void* that, const datResource& rsc)
    {
        new(that) ptxEffectRuleStd(rsc);
    }

    void ptxEffectRule::WriteToJsonBase(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        if(mName)
        {
            writer.String("Name");
            writer.String(mName);
        }

        writer.String("Version");
        writer.Uint(100);

        writer.String("KFColorTint");
        mKFColorTint.WriteToJson(writer);
        writer.String("KFColorTintMax");
        mKFColorTintMax.WriteToJson(writer);
        writer.String("KFZoom");
        mKFZoom.WriteToJson(writer);
        writer.String("KFRotation");
        mKFRotation.WriteToJson(writer);
        writer.String("KFDataSphere");
        mKFDataSphere.WriteToJson(writer);
        writer.String("KFDataCapsuleA");
        mKFDataCapsuleA.WriteToJson(writer);

        writer.String("field_FC");
        writer.Int(field_FC);

        if(mPtxEvoGroup.Get())
        {
            writer.String("PtxEvoGroup");
            mPtxEvoGroup->WriteToJson(writer);
        }

        writer.String("field_104");
        writer.Double((double)field_104);

        writer.String("ZoomCullDistance");
        writer.Int(mZoomCullDistance);

        writer.String("UseRandomColor");
        writer.Bool(mUseRandomColor);
        writer.String("UseDefaultFunctors");
        writer.Bool(mUseDefaultFunctors);
        writer.String("HasDataSphere");
        writer.Bool(mHasDataSphere);

        writer.String("DataObjectType");
        writer.Int(mDataObjectType);

        writer.String("GameFlags");
        writer.Uint(mGameFlags);

        writer.String("field_114");
        writer.Int(field_114);
        writer.String("field_11F");
        writer.Int(field_11F);
    }

    void ptxEffectRule::LoadFromJsonBase(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        if(object.HasMember("Name"))
        {
            mName = strdup(object["Name"].GetString());
        }

        JsonHelpers::LoadMemberObject(mKFColorTint, object, "KFColorTint");
        JsonHelpers::LoadMemberObject(mKFColorTintMax, object, "KFColorTintMax");
        JsonHelpers::LoadMemberObject(mKFZoom, object, "KFZoom");
        JsonHelpers::LoadMemberObject(mKFRotation, object, "KFRotation");
        JsonHelpers::LoadMemberObject(mKFDataSphere, object, "KFDataSphere");
        JsonHelpers::LoadMemberObject(mKFDataCapsuleA, object, "KFDataCapsuleA");

        field_FC = object["field_FC"].GetInt();

        if(object.HasMember("PtxEvoGroup") && object["PtxEvoGroup"].IsObject())
        {
            auto evoGroupObject = object["PtxEvoGroup"].GetObject();
            mPtxEvoGroup = {new ptxEvolutionGroup()};
            mPtxEvoGroup->LoadFromJson(evoGroupObject);
        }

        field_104 = object["field_104"].GetFloat();

        mZoomCullDistance = object["ZoomCullDistance"].GetInt();

        mUseRandomColor = object["UseRandomColor"].GetBool();
        mUseDefaultFunctors = object["UseDefaultFunctors"].GetBool();
        mHasDataSphere = object["HasDataSphere"].GetBool();

        mDataObjectType = object["DataObjectType"].GetInt();

        mGameFlags = object["GameFlags"].GetUint();

        field_114 = object["field_114"].GetInt();
        field_11F = object["field_11F"].GetInt();
    }


    void ptxEffectRuleStd::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mTimeline.AddToLayout(layout, depth);
    }

    void ptxEffectRuleStd::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mTimeline.SerializePtrs(layout, rsc, depth);
    }

    void ptxEffectRuleStd::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            WriteToJsonBase(writer);

            writer.String("Timeline");
            mTimeline.WriteToJson(writer);

            writer.String("FadeDistance");
            writer.Double((double)mFadeDistance);
            writer.String("CullRadius");
            writer.Double((double)mCullRadius);
            writer.String("CullDistance");
            writer.Double((double)mCullDistance);
            writer.String("LodNearDistance");
            writer.Double((double)mLodNearDistance);
            writer.String("LodFarDistance");
            writer.Double((double)mLodFarDistance);
            writer.String("DurationMin");
            writer.Double((double)mDurationMin);
            writer.String("DurationMax");
            writer.Double((double)mDurationMax);
            writer.String("TimeScalarMin");
            writer.Double((double)mTimeScalarMin);
            writer.String("TimeScalarMax");
            writer.Double((double)mTimeScalarMax);

            writer.String("field_16C");
            writer.Int(field_16C);
            writer.String("field_170");
            writer.Int(field_170);

            writer.String("UseCullSphere");
            writer.Bool(mUseCullSphere);
            writer.String("CullNoUpdate");
            writer.Bool(mCullNoUpdate);
            writer.String("CullNoEmit");
            writer.Bool(mCullNoEmit);
            writer.String("CullNoDraw");
            writer.Bool(mCullNoDraw);
            writer.String("SortEvents");
            writer.Bool(mSortEvents);

            writer.String("Quality");
            writer.Int(mQuality);

            writer.String("CullSphere");
            writer.Double((double)mCullSphere);
            writer.String("field_184");
            writer.Double((double)field_184);
            writer.String("field_188");
            writer.Double((double)field_188);

            writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
            writer.String("RandomOffsetPos");
            writer.StartArray();
            {
                writer.Double((double)mRandomOffsetPos.x);
                writer.Double((double)mRandomOffsetPos.y);
                writer.Double((double)mRandomOffsetPos.z);
            }
            writer.EndArray();
            writer.SetFormatOptions(rapidjson::kFormatDefault);

            writer.String("field_19F");
            writer.Int(field_19F);
        }
        writer.EndObject();
    }

    void ptxEffectRuleStd::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        LoadFromJsonBase(object);

        JsonHelpers::LoadMemberObject(mTimeline, object, "Timeline");

        mFadeDistance = object["FadeDistance"].GetFloat();
        mCullRadius = object["CullRadius"].GetFloat();
        mCullDistance = object["CullDistance"].GetFloat();
        mLodNearDistance = object["LodNearDistance"].GetFloat();
        mLodFarDistance = object["LodFarDistance"].GetFloat();
        mDurationMin = object["DurationMin"].GetFloat();
        mDurationMax = object["DurationMax"].GetFloat();
        mTimeScalarMin = object["TimeScalarMin"].GetFloat();
        mTimeScalarMax = object["TimeScalarMax"].GetFloat();

        field_16C = object["field_16C"].GetInt();
        field_170 = object["field_170"].GetInt();

        mUseCullSphere = object["UseCullSphere"].GetBool();
        mCullNoUpdate = object["CullNoUpdate"].GetBool();
        mCullNoEmit = object["CullNoEmit"].GetBool();
        mCullNoDraw = object["CullNoDraw"].GetBool();
        mSortEvents = object["SortEvents"].GetBool();

        mQuality = object["Quality"].GetInt();

        mCullSphere = object["CullSphere"].GetFloat();
        field_184 = object["field_184"].GetFloat();
        field_188 = object["field_188"].GetFloat();

        mRandomOffsetPos.x = object["RandomOffsetPos"].GetArray()[0].GetFloat();;
        mRandomOffsetPos.y = object["RandomOffsetPos"].GetArray()[1].GetFloat();;
        mRandomOffsetPos.z = object["RandomOffsetPos"].GetArray()[2].GetFloat();;

        field_19F = object["field_19F"].GetInt();
    }
}