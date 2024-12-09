#pragma once
#include "rapidjson/include/prettywriter.h"
#include "PtxKeyFrame.h"
#include "../DatOwner.h"
#include "../math/Matrix.h"

namespace rage
{
    class ptxDomain : datBase
    {
    public:
        enum class eDomainType
        {
            BOX = 0,
            SPHERE,
            CYLINDER,
            VORTEX,
            COUNT
        };

    public:
        ptxDomain(const datResource& rsc) : mPositionKF(rsc), mDirectionKF(rsc), mSizeKF(rsc), mInnerSize(rsc), field_11C(rsc), field_120(0) {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        void Place(void* that, const datResource& rsc);

        static const char* TypeToString(eDomainType type);
        static eDomainType StringToType(const char* str);
        uint32_t GetObjectSize() const;

        virtual void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) = 0;

        float field_4;
        eDomainType mType;
        //emitter and velocity?
        int32_t mDomainFunction;
        rmPtfxKeyframe mPositionKF;
        rmPtfxKeyframe mDirectionKF;
        rmPtfxKeyframe mSizeKF;
        rmPtfxKeyframe mInnerSize;
        Matrix34 field_B0;
        Vector3 field_F0;
        int8_t field_FC[4];
        float field_100;
        float field_104;
        float field_108;
        float field_10C;
        float field_110;
        float field_114;
        float mFileVersion;
        //unknown type - always null
        datOwner<void*> field_11C;
        int32_t field_120;
        bool mWorldSpace;
        bool mPointRelative;
        int8_t field_126[10];
    protected:
        void WriteToJsonBase(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
    };
    ASSERT_SIZE(ptxDomain, 0x130);

    //uses mPositionKF, mDirectionKF, mSizeKF and mInnerSize
    class ptxDomainBox : public ptxDomain
    {
    public:
        ptxDomainBox(const datResource& rsc) : ptxDomain(rsc) {}

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;

        float field_130;
        float field_134;
        float field_138;
        int8_t field_13C[4];
        float field_140;
        float field_144;
        float field_148;
        int8_t field_14C[4];
        float field_150;
        float field_154;
        float field_158;
        int8_t field_15C[4];
    };
    ASSERT_SIZE(ptxDomainBox, 0x160);

    //uses mPositionKF, and mSizeKF
    class ptxDomainSphere : public ptxDomain
    {
    public:
        ptxDomainSphere(const datResource& rsc) : ptxDomain(rsc) {}

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;

        float field_130;
        float field_134;
        float field_138;
        int8_t field_13C[4];
    };
    ASSERT_SIZE(ptxDomainSphere, 0x140);

    //uses mPositionKF, mDirectionKF, mSizeKF and mInnerSize
    class ptxDomainCylinder : public ptxDomain
    {
    public:
        ptxDomainCylinder(const datResource& rsc) : ptxDomain(rsc) {}

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;

        float field_130;
        float field_134;
        float field_138;
        int8_t field_13C[4];
        float field_140;
        float field_144;
        float field_148;
        int8_t field_14C[4];
        float field_150;
        float field_154;
        float field_158;
        int8_t field_15C[4];
    };
    ASSERT_SIZE(ptxDomainCylinder, 0x160);

    //uses mPositionKF, mDirectionKF and mSizeKF
    class ptxDomainVortex : public ptxDomain
    {
    public:
        ptxDomainVortex(const datResource& rsc) : ptxDomain(rsc) {}

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;

        float field_130;
        float field_134;
        float field_138;
        int8_t field_13C[4];
    };
    ASSERT_SIZE(ptxDomainVortex, 0x140);
}