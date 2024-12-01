#include "PtxEffectRule.h"

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
            writer.String("ield_184");
            writer.Double((double)field_184);
            writer.String("ield_188");
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
}