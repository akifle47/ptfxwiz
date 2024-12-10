#pragma once
#include "rapidjson/include/prettywriter.h"
#include "JsonHelpers.h"
#include "../Base.h"
#include "../DatRef.h"
#include "PtxKeyFrame.h"
#include "../grmodel/Drawable.h"

#undef GetObject

namespace rage
{
    class ptxBiasLink
    {
    public:
        ptxBiasLink() = default;

        ptxBiasLink(const datResource& rsc) : mPropIDs(rsc), field_48() {}

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) ptxBiasLink(rsc);
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mPropIDs.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mPropIDs.SerializePtrs(layout, rsc, depth);
        }

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
        {
            writer.StartObject();
            {
                writer.String("Name");
                writer.String(mName);

                if(mPropIDs.GetCount())
                {
                    writer.String("PropIDs");
                    writer.StartArray();
                    {
                        for(uint16_t i = 0; i < mPropIDs.GetCount(); i++)
                        {
                            writer.Uint(mPropIDs[i]);
                        }
                    }
                    writer.EndArray();
                }
            }
            writer.EndObject();
        }

        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
        {
            strncpy(mName, object["Name"].GetString(), 64);

            if(object.HasMember("PropIDs") && object["PropIDs"].IsArray())
            {
                auto propIDsArray = object["PropIDs"].GetArray();
                mPropIDs = {(uint16_t)propIDsArray.Size()};
                for(auto& propID : propIDsArray)
                {
                    mPropIDs.Append() = propID.GetUint();
                }
            }
        }

        char mName[64];
        atArray<uint32_t> mPropIDs;
        int8_t field_48;
        int8_t field_49[3];
    };
    ASSERT_SIZE(ptxBiasLink, 0x4C);


    class rmPtxfxProp
    {
    public:
        rmPtxfxProp() : mKeyFrame(), field_28(-1), field_2C(&field_30), field_30(0), field_31(0) 
        {}

        rmPtxfxProp(const datResource& rsc) : mKeyFrame(rsc), field_2C(rsc) 
        {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mKeyFrame.AddToLayout(layout, depth);
            field_2C.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mKeyFrame.SerializePtrs(layout, rsc, depth);
            field_2C.SerializePtrs(layout, rsc, depth);
        }

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
        {
            writer.StartObject();
            {
                writer.String("KeyFrame");
                mKeyFrame.WriteToJson(writer);

                writer.String("field_28");
                writer.Int(field_28);
            }
            writer.EndObject();
        }

        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
        {
            JsonHelpers::LoadMemberObject(mKeyFrame, object, "KeyFrame");

            field_28 = object["field_28"].GetInt();
        }

        rmPtfxKeyframe mKeyFrame;
        int32_t field_28;
        datOwner<int8_t> field_2C;
        int8_t field_30;
        int8_t field_31;
        //2 bytes of padding
    };
    ASSERT_SIZE(rmPtxfxProp, 0x34);


    class ptxRulePropList : public datBase
    {
    public:
        ptxRulePropList() = default;

        ptxRulePropList(const datResource& rsc) : mBiasLinks(rsc), mColorKF(rsc), mColorMaxKF(rsc), mAccelerationMinKF(rsc), mAccelerationMaxKF(rsc),
                                                  mDampeningMinKF(rsc), mDampeningMaxKF(rsc), mMatrixWeightKF(rsc), mPlaybackRateKF(rsc), 
                                                  mAlphaKF(rsc), mPositionNoiseKF(rsc), mVelocityNoiseKF(rsc), mCollisionVelocityDampeningKF(rsc), 
                                                  mCollisionImpVarKF(rsc), mWindInfluenceKF(rsc), mVortexPropsKF(rsc) {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mBiasLinks.AddToLayout(layout, depth);

            mColorKF.AddToLayout(layout, depth);
            mColorMaxKF.AddToLayout(layout, depth);
            mAccelerationMinKF.AddToLayout(layout, depth);
            mAccelerationMaxKF.AddToLayout(layout, depth);
            mDampeningMinKF.AddToLayout(layout, depth);
            mDampeningMaxKF.AddToLayout(layout, depth);
            mMatrixWeightKF.AddToLayout(layout, depth);
            mPlaybackRateKF.AddToLayout(layout, depth);
            mAlphaKF.AddToLayout(layout, depth);
            mPositionNoiseKF.AddToLayout(layout, depth);
            mVelocityNoiseKF.AddToLayout(layout, depth);
            mCollisionVelocityDampeningKF.AddToLayout(layout, depth);
            mCollisionImpVarKF.AddToLayout(layout, depth);
            mWindInfluenceKF.AddToLayout(layout, depth);
            mVortexPropsKF.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mBiasLinks.SerializePtrs(layout, rsc, depth);

            mColorKF.SerializePtrs(layout, rsc, depth);
            mColorMaxKF.SerializePtrs(layout, rsc, depth);
            mAccelerationMinKF.SerializePtrs(layout, rsc, depth);
            mAccelerationMaxKF.SerializePtrs(layout, rsc, depth);
            mDampeningMinKF.SerializePtrs(layout, rsc, depth);
            mDampeningMaxKF.SerializePtrs(layout, rsc, depth);
            mMatrixWeightKF.SerializePtrs(layout, rsc, depth);
            mPlaybackRateKF.SerializePtrs(layout, rsc, depth);
            mAlphaKF.SerializePtrs(layout, rsc, depth);
            mPositionNoiseKF.SerializePtrs(layout, rsc, depth);
            mVelocityNoiseKF.SerializePtrs(layout, rsc, depth);
            mCollisionVelocityDampeningKF.SerializePtrs(layout, rsc, depth);
            mCollisionImpVarKF.SerializePtrs(layout, rsc, depth);
            mWindInfluenceKF.SerializePtrs(layout, rsc, depth);
            mVortexPropsKF.SerializePtrs(layout, rsc, depth);
        }

        virtual void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) = 0;
        virtual void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object) = 0;

        atArray<ptxBiasLink> mBiasLinks;
        int8_t field_C[4];
        rmPtxfxProp mColorKF;
        rmPtxfxProp mColorMaxKF;
        rmPtxfxProp mAccelerationMinKF;
        rmPtxfxProp mAccelerationMaxKF;
        rmPtxfxProp mDampeningMinKF;
        rmPtxfxProp mDampeningMaxKF;
        rmPtxfxProp mMatrixWeightKF;
        rmPtxfxProp mPlaybackRateKF;
        rmPtxfxProp mAlphaKF;
        rmPtxfxProp mPositionNoiseKF;
        rmPtxfxProp mVelocityNoiseKF;
        rmPtxfxProp mCollisionVelocityDampeningKF;
        rmPtxfxProp mCollisionImpVarKF;
        rmPtxfxProp mWindInfluenceKF;
        rmPtxfxProp mVortexPropsKF;

    protected:
        void WriteToJsonBase(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
        {
            if(mBiasLinks.GetCount())
            {
                writer.String("BiasLinks");
                writer.StartArray();
                for(uint16_t i = 0; i < mBiasLinks.GetCount(); i++)
                {
                    mBiasLinks[i].WriteToJson(writer);
                }
                writer.EndArray();
            }

            writer.String("ColorKF");
            mColorKF.WriteToJson(writer);
            writer.String("ColorMaxKF");
            mColorMaxKF.WriteToJson(writer);
            writer.String("AccelerationMinKF");
            mAccelerationMinKF.WriteToJson(writer);
            writer.String("AccelerationMaxKF");
            mAccelerationMaxKF.WriteToJson(writer);
            writer.String("DampeningMinKF");
            mDampeningMinKF.WriteToJson(writer);
            writer.String("DampeningMaxKF");
            mDampeningMaxKF.WriteToJson(writer);
            writer.String("MatrixWeightKF");
            mMatrixWeightKF.WriteToJson(writer);
            writer.String("PlaybackRateKF");
            mPlaybackRateKF.WriteToJson(writer);
            writer.String("AlphaKF");
            mAlphaKF.WriteToJson(writer);
            writer.String("PositionNoiseKF");
            mPositionNoiseKF.WriteToJson(writer);
            writer.String("VelocityNoiseKF");
            mVelocityNoiseKF.WriteToJson(writer);
            writer.String("CollisionVelocityDampeningKF");
            mCollisionVelocityDampeningKF.WriteToJson(writer);
            writer.String("CollisionImpVarKF");
            mCollisionImpVarKF.WriteToJson(writer);
            writer.String("WindInfluenceKF");
            mWindInfluenceKF.WriteToJson(writer);
            writer.String("VortexPropsKF");
            mVortexPropsKF.WriteToJson(writer);
        }

        void LoadFromJsonBase(rapidjson::GenericObject<true, rapidjson::Value>& object)
        {
            if(object.HasMember("BiasLinks"))
            {
                if(object["BiasLinks"].IsArray())
                {
                    auto biasLinksArray = object["BiasLinks"].GetArray();
                    mBiasLinks = {(uint16_t)biasLinksArray.Size()};
                    for(const auto& biasLink : biasLinksArray)
                    {
                        auto biasLinkObject = biasLink.GetObject();
                        mBiasLinks.Append().LoadFromJson(biasLinkObject);
                    }
                }
            }

            JsonHelpers::LoadMemberObject(mColorKF, object, "ColorKF");
            JsonHelpers::LoadMemberObject(mColorMaxKF, object, "ColorMaxKF");
            JsonHelpers::LoadMemberObject(mAccelerationMinKF, object, "AccelerationMinKF");
            JsonHelpers::LoadMemberObject(mAccelerationMaxKF, object, "AccelerationMaxKF");
            JsonHelpers::LoadMemberObject(mDampeningMinKF, object, "DampeningMinKF");
            JsonHelpers::LoadMemberObject(mDampeningMaxKF, object, "DampeningMaxKF");
            JsonHelpers::LoadMemberObject(mMatrixWeightKF, object, "MatrixWeightKF");
            JsonHelpers::LoadMemberObject(mPlaybackRateKF, object, "PlaybackRateKF");
            JsonHelpers::LoadMemberObject(mAlphaKF, object, "AlphaKF");
            JsonHelpers::LoadMemberObject(mPositionNoiseKF, object, "PositionNoiseKF");
            JsonHelpers::LoadMemberObject(mVelocityNoiseKF, object, "VelocityNoiseKF");
            JsonHelpers::LoadMemberObject(mCollisionVelocityDampeningKF, object, "CollisionVelocityDampeningKF");
            JsonHelpers::LoadMemberObject(mCollisionImpVarKF, object, "CollisionImpVarKF");
            JsonHelpers::LoadMemberObject(mWindInfluenceKF, object, "WindInfluenceKF");
            JsonHelpers::LoadMemberObject(mVortexPropsKF, object, "VortexPropsKF");
        }
    };
    ASSERT_SIZE(ptxRulePropList, 0x31C);


    class ptxSpriteRulePropList : public ptxRulePropList
    {
    public:
        ptxSpriteRulePropList() = default;

        ptxSpriteRulePropList(const datResource& rsc) : ptxRulePropList(rsc), mSizeKF(rsc), mThetaKF(rsc), mInitThetaKFOT(rsc),
                                                        mInitRotateVelKFOT(rsc), mRotateVelKF(rsc), mDirectionalKF(rsc), mDirectionalVelKF(rsc), 
                                                        mTextureAnimRateKF(rsc), mTrailPropsKF(rsc) {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            ptxRulePropList::AddToLayout(layout, depth);

            mSizeKF.AddToLayout(layout, depth);
            mThetaKF.AddToLayout(layout, depth);
            mInitThetaKFOT.AddToLayout(layout, depth);
            mInitRotateVelKFOT.AddToLayout(layout, depth);
            mRotateVelKF.AddToLayout(layout, depth);
            mDirectionalKF.AddToLayout(layout, depth);
            mDirectionalVelKF.AddToLayout(layout, depth);
            mTextureAnimRateKF.AddToLayout(layout, depth);
            mTrailPropsKF.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            ptxRulePropList::SerializePtrs(layout, rsc, depth);

            mSizeKF.SerializePtrs(layout, rsc, depth);
            mThetaKF.SerializePtrs(layout, rsc, depth);
            mInitThetaKFOT.SerializePtrs(layout, rsc, depth);
            mInitRotateVelKFOT.SerializePtrs(layout, rsc, depth);
            mRotateVelKF.SerializePtrs(layout, rsc, depth);
            mDirectionalKF.SerializePtrs(layout, rsc, depth);
            mDirectionalVelKF.SerializePtrs(layout, rsc, depth);
            mTextureAnimRateKF.SerializePtrs(layout, rsc, depth);
            mTrailPropsKF.SerializePtrs(layout, rsc, depth);
        }

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
        {
            writer.StartObject();
            {
                WriteToJsonBase(writer);

                writer.String("SizeKF");
                mSizeKF.WriteToJson(writer);
                writer.String("ThetaKF");
                mThetaKF.WriteToJson(writer);
                writer.String("InitThetaKFOT");
                mInitThetaKFOT.WriteToJson(writer);
                writer.String("InitRotateVelKFOT");
                mInitRotateVelKFOT.WriteToJson(writer);
                writer.String("RotateVelKF");
                mRotateVelKF.WriteToJson(writer);
                writer.String("DirectionalKF");
                mDirectionalKF.WriteToJson(writer);
                writer.String("DirectionalVelKF");
                mDirectionalVelKF.WriteToJson(writer);
                writer.String("TextureAnimRateKF");
                mTextureAnimRateKF.WriteToJson(writer);
                writer.String("TrailPropsKF");
                mTrailPropsKF.WriteToJson(writer);
            }
            writer.EndObject();
        }

        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object) override
        {
            LoadFromJsonBase(object);

            JsonHelpers::LoadMemberObject(mSizeKF, object, "SizeKF");
            JsonHelpers::LoadMemberObject(mThetaKF, object, "ThetaKF");
            JsonHelpers::LoadMemberObject(mInitThetaKFOT, object, "InitThetaKFOT");
            JsonHelpers::LoadMemberObject(mInitRotateVelKFOT, object, "InitRotateVelKFOT");
            JsonHelpers::LoadMemberObject(mRotateVelKF, object, "RotateVelKF");
            JsonHelpers::LoadMemberObject(mDirectionalKF, object, "DirectionalKF");
            JsonHelpers::LoadMemberObject(mDirectionalVelKF, object, "DirectionalVelKF");
            JsonHelpers::LoadMemberObject(mTextureAnimRateKF, object, "TextureAnimRateKF");
            JsonHelpers::LoadMemberObject(mTrailPropsKF, object, "TrailPropsKF");
        }

        rmPtxfxProp mSizeKF;
        rmPtxfxProp mThetaKF;
        rmPtxfxProp mInitThetaKFOT;
        rmPtxfxProp mInitRotateVelKFOT;
        rmPtxfxProp mRotateVelKF;
        rmPtxfxProp mDirectionalKF;
        rmPtxfxProp mDirectionalVelKF;
        rmPtxfxProp mTextureAnimRateKF;
        rmPtxfxProp mTrailPropsKF;
    };
    ASSERT_SIZE(ptxSpriteRulePropList, 0x4F0);


    class ptxModelRulePropList : public ptxRulePropList
    {
    public:
        ptxModelRulePropList() = default;

        ptxModelRulePropList(const datResource& rsc) : ptxRulePropList(rsc), mSizeMin(rsc), mSizeMax(rsc), mInitialThetaMin(rsc), 
                                                       mInitialThetaMax(rsc), mThetaMin(rsc), mThetaMax(rsc), mInitialRotationMin(rsc), 
                                                       mInitialRotationMax(rsc), mInitRotationSpeed(rsc), mRotationSpeed(rsc) {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            ptxRulePropList::AddToLayout(layout, depth);

            mSizeMin.AddToLayout(layout, depth);
            mSizeMax.AddToLayout(layout, depth);
            mInitialThetaMin.AddToLayout(layout, depth);
            mInitialThetaMax.AddToLayout(layout, depth);
            mThetaMin.AddToLayout(layout, depth);
            mThetaMax.AddToLayout(layout, depth);
            mInitialRotationMin.AddToLayout(layout, depth);
            mInitialRotationMax.AddToLayout(layout, depth);
            mInitRotationSpeed.AddToLayout(layout, depth);
            mRotationSpeed.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            ptxRulePropList::SerializePtrs(layout, rsc, depth);

            mSizeMin.SerializePtrs(layout, rsc, depth);
            mSizeMax.SerializePtrs(layout, rsc, depth);
            mInitialThetaMin.SerializePtrs(layout, rsc, depth);
            mInitialThetaMax.SerializePtrs(layout, rsc, depth);
            mThetaMin.SerializePtrs(layout, rsc, depth);
            mThetaMax.SerializePtrs(layout, rsc, depth);
            mInitialRotationMin.SerializePtrs(layout, rsc, depth);
            mInitialRotationMax.SerializePtrs(layout, rsc, depth);
            mInitRotationSpeed.SerializePtrs(layout, rsc, depth);
            mRotationSpeed.SerializePtrs(layout, rsc, depth);
        }

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
        {
            writer.StartObject();
            {
                WriteToJsonBase(writer);

                writer.String("SizeMin");
                mSizeMin.WriteToJson(writer);
                writer.String("SizeMax");
                mSizeMax.WriteToJson(writer);
                writer.String("InitialThetaMin");
                mInitialThetaMin.WriteToJson(writer);
                writer.String("InitialThetaMax");
                mInitialThetaMax.WriteToJson(writer);
                writer.String("ThetaMin");
                mThetaMin.WriteToJson(writer);
                writer.String("ThetaMax");
                mThetaMax.WriteToJson(writer);
                writer.String("InitialRotationMin");
                mInitialRotationMin.WriteToJson(writer);
                writer.String("InitialRotationMax");
                mInitialRotationMax.WriteToJson(writer);
                writer.String("InitRotationSpeed");
                mInitRotationSpeed.WriteToJson(writer);
                writer.String("RotationSpeed");
                mRotationSpeed.WriteToJson(writer);
            }
            writer.EndObject();
        }

        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object) override
        {
            LoadFromJsonBase(object);

            JsonHelpers::LoadMemberObject(mSizeMin, object, "SizeMin");
            JsonHelpers::LoadMemberObject(mSizeMax, object, "SizeMax");
            JsonHelpers::LoadMemberObject(mInitialThetaMin, object, "InitialThetaMin");
            JsonHelpers::LoadMemberObject(mInitialThetaMax, object, "InitialThetaMax");
            JsonHelpers::LoadMemberObject(mThetaMin, object, "ThetaMin");
            JsonHelpers::LoadMemberObject(mThetaMax, object, "ThetaMax");
            JsonHelpers::LoadMemberObject(mInitialRotationMin, object, "InitialRotationMin");
            JsonHelpers::LoadMemberObject(mInitialRotationMax, object, "InitialRotationMax");
            JsonHelpers::LoadMemberObject(mInitRotationSpeed, object, "InitRotationSpeed");
            JsonHelpers::LoadMemberObject(mRotationSpeed, object, "RotationSpeed");
        }

        rmPtxfxProp mSizeMin;
        rmPtxfxProp mSizeMax;
        rmPtxfxProp mInitialThetaMin;
        rmPtxfxProp mInitialThetaMax;
        rmPtxfxProp mThetaMin;
        rmPtxfxProp mThetaMax;
        rmPtxfxProp mInitialRotationMin;
        rmPtxfxProp mInitialRotationMax;
        rmPtxfxProp mInitRotationSpeed;
        rmPtxfxProp mRotationSpeed;
    };
    ASSERT_SIZE(ptxModelRulePropList, 0x524);

    class PtxNameDrawablePair
    {
    public:
        PtxNameDrawablePair() : mName(nullptr) {};

        PtxNameDrawablePair(const datResource& rsc) : mDrawable(rsc) 
        {
            rsc.PointerFixUp(mName);
        }

        ~PtxNameDrawablePair()
        {
            if(mName)
            {
                delete[] mName;
                mName = nullptr;
            }
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            layout.AddObject(mName, RSC5Layout::eBlockType::VIRTUAL, strlen(mName) + 1);
            mDrawable.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            layout.SerializePtr(mName, strlen(mName) + 1);
            mDrawable.SerializePtrs(layout, rsc, depth);
        }

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) PtxNameDrawablePair(rsc);
        }

        char* mName;
        datRef<rmcDrawable> mDrawable;
    };
}