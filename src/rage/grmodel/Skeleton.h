#pragma once
#include "../math/Matrix.h"
#include "../Array.h"
#include "../../Utils.h"

#include <cstdint>

namespace rage
{
    struct crJointControlPoint 
    {
        float MaxSwing;
        float MinTwist;
        float MaxTwist;
    };
    ASSERT_SIZE(crJointControlPoint, 0xC);

    class crJointData
    {
    public:
        virtual ~crJointData() {};

        int32_t mBoneID;
        int32_t mJointDOFs;
        bool mUseTwistLimits;
        bool mUsePerControlTwistLimits;
        int8_t field_E[2];
        int32_t mNumControlPoints;
        int8_t field_14[0xC];
        Vector4 mTwistAxis;
        int mTwistLimitMin;
        int mTwistLimitMax;
        int mSoftLimitScale;
        int8_t field_3C[0x4];
        Vector4 mZeroRotation;
        crJointControlPoint mControlPoints[8];
        Vector4 mZeroRotationEulers;
    };
    ASSERT_SIZE(crJointData, 0xC0);

    class crJointDataFile 
    {
    public:
        virtual ~crJointDataFile() {};

        atArray<crJointData> mJointData;
    };
    ASSERT_SIZE(crJointDataFile, 0xC);

    class crBoneData
    {
    public:
        crBoneData(const datResource& rsc) : mNext(rsc), mChild(rsc), mParent(rsc), mJointData(rsc)
        {
            if(mName)
                rsc.PointerFixUp(mName);
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            layout.AddObject(mName, RSC5Layout::eBlockType::VIRTUAL, strlen(mName) + 1);
            mNext.AddToLayout(layout, depth);
            mChild.AddToLayout(layout, depth);
            mParent.AddToLayout(layout, depth);
            mJointData.AddToLayout(layout, depth);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            layout.SerializePtr(mName, strlen(mName) + 1);

            mNext.SerializePtrs(layout, rsc, depth);
            mChild.SerializePtrs(layout, rsc, depth);
            mParent.SerializePtrs(layout, rsc, depth);
            mJointData.SerializePtrs(layout, rsc, depth);
        }

        char* mName;
        uint32_t mFlags;
        datOwner<crBoneData> mNext;
        datOwner<crBoneData> mChild;
        datOwner<crBoneData> mParent;
        uint16_t mIndex;
        uint16_t mId;
        uint16_t mMirrorIndex;
        uint8_t mTransFlagsCount;
        uint8_t mRotFlagsCount;
        uint8_t mScaleFlagsCount;
        int8_t field_1D[3];
        Vector4 mDefaultTranslation;
        Vector4 mDefaultRotation;
        Vector4 mDefaultRotationQuat;
        Vector4 mDefaultScale;
        Vector4 mGlobalOffset;
        Vector4 mJointOrient;
        Vector4 mScaleOrient;
        Vector4 mTransMin;
        Vector4 mTransMax;
        Vector4 mRotMin;
        Vector4 mRotMax;
        datOwner<void*> mJointData;
        uint32_t mNameHash;
        int32_t field_D8;
        int32_t field_DC;
    };
    ASSERT_SIZE(crBoneData, 0xE0);


    class crSkeletonData
    {
    public:
        crSkeletonData(const datResource& rsc) : mBoneIdMappings(rsc), mParentBoneIndices(rsc), mBoneWorldOrient(rsc), mBoneWorldOrientInverted(rsc),
                                                 mBoneLocalTransforms(rsc)
        {
            if(mBones)
            {
                rsc.PointerFixUp(mBones);

                for(uint16_t i = 0; i < mNumBones; i++)
                {
                    new(mBones) crBoneData(rsc);
                }
            }
        }

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) crSkeletonData(rsc);
        }


        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            mBoneIdMappings.AddToLayout(layout, depth);
            mParentBoneIndices.AddToLayout(layout, depth);
            mBoneWorldOrient.AddToLayout(layout, depth);
            mBoneWorldOrientInverted.AddToLayout(layout, depth);
            mBoneLocalTransforms.AddToLayout(layout, depth);

            if(mBones)
            {
                layout.AddObject(mBones, RSC5Layout::eBlockType::VIRTUAL, mNumBones);

                for(uint16_t i = 0; i < mNumBones; i++)
                {
                    mBones[i].AddToLayout(layout, depth);
                }
            }
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mBoneIdMappings.SerializePtrs(layout, rsc, depth);
            mParentBoneIndices.SerializePtrs(layout, rsc, depth);
            mBoneWorldOrient.SerializePtrs(layout, rsc, depth);
            mBoneWorldOrientInverted.SerializePtrs(layout, rsc, depth);
            mBoneLocalTransforms.SerializePtrs(layout, rsc, depth);
            
            if(mBones)
            {
                for(uint16_t i = 0; i < mNumBones; i++)
                {
                    mBones[i].SerializePtrs(layout, rsc, depth);
                }

                layout.SerializePtr(mBones, sizeof(crBoneData) * mNumBones);
            }
        }

        struct BoneIdData
        {
            uint16_t ID;
            uint16_t Index;
        };

        crBoneData* mBones;
        datOwner<int32_t> mParentBoneIndices;
        datOwner<Matrix34> mBoneWorldOrient;
        datOwner<Matrix34> mBoneWorldOrientInverted;
        datOwner<Matrix34> mBoneLocalTransforms;
        uint16_t mNumBones;
        uint16_t mTransLockCount;
        uint16_t mRotLockCount;
        uint16_t mScaleLockCount;
        uint32_t mFlags;
        atArray<BoneIdData> mBoneIdMappings;
        uint32_t mUsageCount;
        uint32_t mCRC;
        //i think the game always zeroes both of these?
        char* mJointDataFileName;
        crJointDataFile mJointData;
    };
    ASSERT_SIZE(crSkeletonData, 0x40);
}