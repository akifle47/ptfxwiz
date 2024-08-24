#pragma once
#include "../math/Matrix.h"
#include "../Array.h"
#include "../DatRef.h"
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
        friend class crSkeletonData;

        crBoneData(const datResource& rsc) : mNext(rsc), mChild(rsc), mParent(rsc), mJointData(rsc)
        {
            if(mName)
                rsc.PointerFixUp(mName);
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            if(mName)
                layout.AddObject(mName, RSC5Layout::eBlockType::VIRTUAL, strlen(mName) + 1);
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            layout.SerializePtr(mName, strlen(mName) + 1);
        }

        char* mName;
        uint32_t mFlags;
        datRef<crBoneData> mNext;
        datRef<crBoneData> mChild;
        datRef<crBoneData> mParent;
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
                    if(&mBones[i])
                        new(&mBones[i]) crBoneData(rsc);
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

            layout.AddObject(mParentBoneIndices.Get(), RSC5Layout::eBlockType::VIRTUAL, mNumBones);

            if(mBoneWorldOrientInverted.Get())
                layout.AddObject(mBoneWorldOrientInverted.Get(), RSC5Layout::eBlockType::VIRTUAL, mNumBones * 2);

            layout.AddObject(mBoneLocalTransforms.Get(), RSC5Layout::eBlockType::VIRTUAL, mNumBones);

            if(mBones)
            {
                layout.AddObject(mBones, RSC5Layout::eBlockType::VIRTUAL, mNumBones);

                for(uint16_t i = 0; i < mNumBones; i++)
                {
                    mBones[i].AddToLayout(layout, depth + 1);
                }
            }
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            mBoneIdMappings.SerializePtrs(layout, rsc, depth);

            layout.SerializePtr(mParentBoneIndices.Get(), sizeof(int32_t) * mNumBones);

            if(mBoneWorldOrientInverted.Get())
            {
                layout.SerializePtr(mBoneWorldOrientInverted.Get(), sizeof(Matrix34) * mNumBones * 2);

                layout.BackupPtr(mBoneWorldOrient.Get());
                mBoneWorldOrient.Get() = (Matrix34*)((uintptr_t)mBoneWorldOrientInverted.Get() + sizeof(Matrix34) * mNumBones);
            }

            layout.SerializePtr(mBoneLocalTransforms.Get(), sizeof(Matrix34) * mNumBones);
            
            if(mBones)
            {
                for(uint16_t i = 0; i < mNumBones; i++)
                {
                    mBones[i].SerializePtrs(layout, rsc, depth);
                }

                layout.SerializePtr(mBones, sizeof(crBoneData) * mNumBones);
                crBoneData* bonesRsc = (crBoneData*)((uint8_t*)(mBones) + rsc.GetFixUp((void*)mBones));

                for(uint16_t i = 0; i < mNumBones; i++)
                {
                    if(bonesRsc[i].mNext.Get())
                        bonesRsc[i].mNext.Get() = (crBoneData*)((uint32_t)mBones + sizeof(crBoneData) * bonesRsc[i].mNext->mIndex);
                    if(bonesRsc[i].mChild.Get())
                        bonesRsc[i].mChild.Get() = (crBoneData*)((uint32_t)mBones + sizeof(crBoneData) * bonesRsc[i].mChild->mIndex);
                    if(bonesRsc[i].mParent.Get())
                        bonesRsc[i].mParent.Get() = (crBoneData*)((uint32_t)mBones + sizeof(crBoneData) * bonesRsc[i].mParent->mIndex);
                }
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
        union
        {
            uint32_t mUnkFlag0 : 1;
            uint32_t mHasBoneMappings : 1;
            uint32_t mHasBoneWorldOrient : 1;
            uint32_t mAuthoredOrientation : 1;
            uint32_t mUnkFlag4 : 1;
            uint32_t mUnkFlag5 : 27;
        };
        atArray<BoneIdData> mBoneIdMappings;
        uint32_t mUsageCount;
        uint32_t mCRC;
        //i think the game always zeroes both of these?
        char* mJointDataFileName;
        crJointDataFile mJointData;
    };
    ASSERT_SIZE(crSkeletonData, 0x40);
}