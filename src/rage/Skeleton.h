#pragma once
#include "Matrix.h"
#include "Array.h"
#include "../Utils.h"

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
        char* mName;
        uint32_t mFlags;
        crBoneData* mNext;
        crBoneData* mChild;
        crBoneData* mParent;
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
        void* mJointData;
        uint32_t mNameHash;
        int32_t field_D8;
        int32_t field_DC;
    };
    ASSERT_SIZE(crBoneData, 0xE0);

    class crSkeletonData
    {
    public:
        struct BoneIdData
        {
            uint16_t ID;
            uint16_t Index;
        };

        crBoneData* mBones;
        int32_t mParentBoneIndices;
        Matrix34* mBoneWorldOrient;
        Matrix34* mBoneWorldOrientInverted;
        Matrix34* mBoneLocalTransforms;
        uint16_t mNumBones;
        uint16_t mTransLockCount;
        uint16_t mRotLockCount;
        uint16_t mScaleLockCount;
        uint32_t mFlags;
        atArray<BoneIdData> mBoneIdMappings;
        uint32_t mUsageCount;
        uint32_t mCRC;
        char* mJointDataFileName;
        crJointDataFile mJointData;
    };
    ASSERT_SIZE(crSkeletonData, 0x40);
}