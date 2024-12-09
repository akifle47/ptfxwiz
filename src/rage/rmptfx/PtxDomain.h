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
        ptxDomain(uint32_t domainFunction, eDomainType domainType) 
            : field_4(), mType(domainType), mDomainFunction(domainFunction), field_B0(gMatrix34Identity), field_FC{0}, field_100(0.0f), field_104(0.0f), field_108(0.0f),
              field_10C(0.0f), field_110(0.0f), field_114(-1.0f), mFileVersion(2.0f), field_11C(nullptr), field_120(0), mWorldSpace(false), mPointRelative(false), field_126{} 
        {}

        ptxDomain(const datResource& rsc) : mPositionKF(rsc), mDirectionKF(rsc), mSizeKF(rsc), mInnerSize(rsc), field_11C(rsc), field_120(0) {}

        void AddToLayout(RSC5Layout& layout, uint32_t depth);
        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth);

        void Place(void* that, const datResource& rsc);

        static const char* TypeToString(eDomainType type);
        static eDomainType StringToType(const char* str);
        uint32_t GetObjectSize() const;

        virtual void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) = 0;
        virtual void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object) = 0;

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
        void LoadFromJsonBase(rapidjson::GenericObject<true, rapidjson::Value>& object);
    };
    ASSERT_SIZE(ptxDomain, 0x130);

    //uses mPositionKF, mDirectionKF, mSizeKF and mInnerSize
    class ptxDomainBox : public ptxDomain
    {
    public:
        ptxDomainBox(uint32_t domainFunction) : ptxDomain(domainFunction, eDomainType::BOX), field_130(0.0f), field_134(0.0f), field_138(0.0f), field_13C{}, 
                                                field_140(0.0f), field_144(0.0f), field_148(0.0f), field_14C{}, field_150(0.0f), field_154(0.0f), field_158(0.0f), field_15C {}
        {}

        ptxDomainBox(const datResource& rsc) : ptxDomain(rsc) {}

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;
        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object) override;

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
        ptxDomainSphere(uint32_t domainFunction) : ptxDomain(domainFunction, eDomainType::SPHERE), field_130(0.0f), field_134(0.0f), field_138(0.0f), field_13C{} {}

        ptxDomainSphere(const datResource& rsc) : ptxDomain(rsc) {}

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;
        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object) override;

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
        ptxDomainCylinder(uint32_t domainFunction) : ptxDomain(domainFunction, eDomainType::CYLINDER), field_130(0.0f), field_134(0.0f), field_138(0.0f), field_13C{}, 
                                                     field_140(0.0f), field_144(0.0f), field_148(0.0f), field_14C{}, field_150(0.0f), field_154(0.0f), field_158(0.0f), field_15C{} {}

        ptxDomainCylinder(const datResource& rsc) : ptxDomain(rsc) {}

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;
        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object) override;

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
        ptxDomainVortex(uint32_t domainFunction) : ptxDomain(domainFunction, eDomainType::VORTEX), field_130(0.0f), field_134(0.0f), field_138(0.0f), field_13C{} {}

        ptxDomainVortex(const datResource& rsc) : ptxDomain(rsc) {}

        void WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override;
        void LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object) override;

        float field_130;
        float field_134;
        float field_138;
        int8_t field_13C[4];
    };
    ASSERT_SIZE(ptxDomainVortex, 0x140);
}