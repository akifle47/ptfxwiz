#include "PtxDomain.h"
#include "JsonHelpers.h"

namespace rage
{
    //nothing special to do in the children because none of them have pointers
    void ptxDomain::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        mPositionKF.AddToLayout(layout, depth);
        mDirectionKF.AddToLayout(layout, depth);
        mSizeKF.AddToLayout(layout, depth);
        mInnerSize.AddToLayout(layout, depth);

        field_11C.AddToLayout(layout, depth);
    }

    void ptxDomain::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        mPositionKF.SerializePtrs(layout, rsc,  depth);
        mDirectionKF.SerializePtrs(layout, rsc,  depth);
        mSizeKF.SerializePtrs(layout, rsc,  depth);
        mInnerSize.SerializePtrs(layout, rsc,  depth);

        field_11C.SerializePtrs(layout, rsc,  depth);
    }

    void ptxDomain::Place(void* that, const datResource& rsc)
    {
        switch(mType)
        {
            case eDomainType::BOX:
                new(that) ptxDomainBox(rsc);
            break;

            case eDomainType::SPHERE:
                new(that) ptxDomainSphere(rsc);
            break;

            case eDomainType::CYLINDER:
                new(that) ptxDomainCylinder(rsc);
            break;

            case eDomainType::VORTEX:
                new(that) ptxDomainVortex(rsc);
            break;

            default:
                Log::Error("Invalid ptx domain type - %d", mType);
        }
    }

    uint32_t ptxDomain::GetObjectSize() const
    {
        switch(mType)
        {
            case eDomainType::BOX:
            return sizeof(ptxDomainBox);
            break;

            case eDomainType::SPHERE:
            return sizeof(ptxDomainSphere);
            break;

            case eDomainType::CYLINDER:
            return sizeof(ptxDomainCylinder);
            break;

            case eDomainType::VORTEX:
            return sizeof(ptxDomainVortex);
            break;

            default:
            Log::Error("Invalid ptx domain type - %d", mType);
        }
    }

    const char* ptxDomain::TypeToString(eDomainType type)
    {
        static const char* lut[] {"BOX", "SPHERE", "CYLINDER", "VORTEX", "COUNT"};

        if((uint32_t)type >= (uint32_t)eDomainType::COUNT)
            return "INVALID_TYPE";
        else
            return lut[(uint32_t)type];
    }

    ptxDomain::eDomainType ptxDomain::StringToType(const char* str)
    {
        static const char* lut[] {"BOX", "SPHERE", "CYLINDER", "VORTEX", "COUNT"};

        if(!str)
            return eDomainType::COUNT;

        for(size_t i = 0; i < std::size(lut); i++)
        {
            if(stricmp(str, lut[i]) == 0)
                return (eDomainType)i;
        }

        return eDomainType::COUNT;
    }

    void ptxDomain::WriteToJsonBase(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.String("field_4");
        writer.Double((double)field_4);

        writer.String("Type");
        writer.String(TypeToString(mType));

        writer.String("DomainFunction");
        writer.Int(mDomainFunction);

        writer.String("PositionKF");
        mPositionKF.WriteToJson(writer);
        writer.String("DirectionKF");
        mDirectionKF.WriteToJson(writer);
        writer.String("SizeKF");
        mSizeKF.WriteToJson(writer);
        writer.String("InnerSize");
        mInnerSize.WriteToJson(writer);

        writer.String("field_B0");
        writer.StartArray();
        {
            writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
            writer.StartArray();
            {
                writer.Double((double)field_B0.a.x);
                writer.Double((double)field_B0.a.y);
                writer.Double((double)field_B0.a.z);
            }
            writer.EndArray();

            writer.StartArray();
            {
                writer.Double((double)field_B0.b.x);
                writer.Double((double)field_B0.b.y);
                writer.Double((double)field_B0.b.z);
            }
            writer.EndArray();

            writer.StartArray();
            {
                writer.Double((double)field_B0.c.x);
                writer.Double((double)field_B0.c.y);
                writer.Double((double)field_B0.c.z);
            }
            writer.EndArray();
        }
        writer.EndArray();

        writer.String("field_F0");
        writer.StartArray();
        {
            writer.Double((double)field_F0.x);
            writer.Double((double)field_F0.y);
            writer.Double((double)field_F0.z);
        }
        writer.EndArray();
        writer.SetFormatOptions(rapidjson::kFormatDefault);

        writer.String("field_100");
        writer.Double((double)field_100);
        writer.String("field_104");
        writer.Double((double)field_104);
        writer.String("field_108");
        writer.Double((double)field_108);
        writer.String("field_10C");
        writer.Double((double)field_10C);
        writer.String("field_110");
        writer.Double((double)field_110);
        writer.String("field_114");
        writer.Double((double)field_114);

        writer.String("field_120");
        writer.Int(field_120);

        writer.String("WorldSpace");
        writer.Bool(mWorldSpace);

        writer.String("PointRelative");
        writer.Bool(mPointRelative);
    }

    void ptxDomain::LoadFromJsonBase(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        field_4 = object["field_4"].GetFloat();

        JsonHelpers::LoadMemberObject(mPositionKF, object, "PositionKF");
        JsonHelpers::LoadMemberObject(mDirectionKF, object, "DirectionKF");
        JsonHelpers::LoadMemberObject(mSizeKF, object, "SizeKF");
        JsonHelpers::LoadMemberObject(mInnerSize, object, "InnerSize");

        auto field_B0_Array = object["field_B0"].GetArray();
        auto field_B0_Row1 = field_B0_Array[0].GetArray();
        auto field_B0_Row2 = field_B0_Array[1].GetArray();
        auto field_B0_Row3 = field_B0_Array[2].GetArray();
        field_B0.a.x = field_B0_Row1[0].GetFloat();
        field_B0.a.y = field_B0_Row1[1].GetFloat();
        field_B0.a.z = field_B0_Row1[2].GetFloat();
        field_B0.b.x = field_B0_Row2[0].GetFloat();
        field_B0.b.y = field_B0_Row2[1].GetFloat();
        field_B0.b.z = field_B0_Row2[2].GetFloat();
        field_B0.c.x = field_B0_Row3[0].GetFloat();
        field_B0.c.y = field_B0_Row3[1].GetFloat();
        field_B0.c.z = field_B0_Row3[2].GetFloat();

        field_F0.x = object["field_F0"].GetArray()[0].GetFloat();
        field_F0.y = object["field_F0"].GetArray()[1].GetFloat();
        field_F0.z = object["field_F0"].GetArray()[2].GetFloat();

        field_100 = object["field_100"].GetFloat();
        field_104 = object["field_104"].GetFloat();
        field_108 = object["field_108"].GetFloat();
        field_10C = object["field_10C"].GetFloat();
        field_110 = object["field_110"].GetFloat();
        field_114 = object["field_114"].GetFloat();

        field_120 = object["field_120"].GetInt();
        mWorldSpace = object["WorldSpace"].GetBool();
        mPointRelative = object["PointRelative"].GetBool();
    }


    void ptxDomainBox::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            WriteToJsonBase(writer);

            writer.String("field_130");
            writer.Double((double)field_130);
            writer.String("field_134");
            writer.Double((double)field_134);
            writer.String("field_138");
            writer.Double((double)field_138);
            writer.String("field_13C");
            writer.Double((double)field_13C);

            writer.String("field_140");
            writer.Double((double)field_140);
            writer.String("field_144");
            writer.Double((double)field_144);
            writer.String("field_148");
            writer.Double((double)field_148);
            writer.String("field_14C");
            writer.Double((double)field_14C);

            writer.String("field_150");
            writer.Double((double)field_150);
            writer.String("field_154");
            writer.Double((double)field_154);
            writer.String("field_158");
            writer.Double((double)field_158);
            writer.String("field_15C");
            writer.Double((double)field_15C);
        }
        writer.EndObject();
    }

    void ptxDomainBox::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        LoadFromJsonBase(object);

        field_130 = object["field_130"].GetFloat();
        field_134 = object["field_134"].GetFloat();
        field_138 = object["field_138"].GetFloat();
        field_13C = object["field_13C"].GetFloat();

        field_140 = object["field_140"].GetFloat();
        field_144 = object["field_144"].GetFloat();
        field_148 = object["field_148"].GetFloat();
        field_14C = object["field_14C"].GetFloat();

        field_150 = object["field_150"].GetFloat();
        field_154 = object["field_154"].GetFloat();
        field_158 = object["field_158"].GetFloat();
        field_15C = object["field_15C"].GetFloat();
    }


    void ptxDomainSphere::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            WriteToJsonBase(writer);

            writer.String("field_130");
            writer.Double((double)field_130);
            writer.String("field_134");
            writer.Double((double)field_134);
            writer.String("field_138");
            writer.Double((double)field_138);
            writer.String("field_13C");
            writer.Double((double)field_13C);
        }
        writer.EndObject();
    }

    void ptxDomainSphere::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        LoadFromJsonBase(object);

        field_130 = object["field_130"].GetFloat();
        field_134 = object["field_134"].GetFloat();
        field_138 = object["field_138"].GetFloat();
        field_13C = object["field_13C"].GetFloat();
    }


    void ptxDomainCylinder::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            WriteToJsonBase(writer);

            writer.String("field_130");
            writer.Double((double)field_130);
            writer.String("field_134");
            writer.Double((double)field_134);
            writer.String("field_138");
            writer.Double((double)field_138);
            writer.String("field_13C");
            writer.Double((double)field_13C);

            writer.String("field_140");
            writer.Double((double)field_140);
            writer.String("field_144");
            writer.Double((double)field_144);
            writer.String("field_148");
            writer.Double((double)field_148);
            writer.String("field_14C");
            writer.Double((double)field_14C);

            writer.String("field_150");
            writer.Double((double)field_150);
            writer.String("field_154");
            writer.Double((double)field_154);
            writer.String("field_158");
            writer.Double((double)field_158);
            writer.String("field_15C");
            writer.Double((double)field_15C);
        }
        writer.EndObject();
    }

    void ptxDomainCylinder::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        LoadFromJsonBase(object);

        field_130 = object["field_130"].GetFloat();
        field_134 = object["field_134"].GetFloat();
        field_138 = object["field_138"].GetFloat();
        field_13C = object["field_13C"].GetFloat();

        field_140 = object["field_140"].GetFloat();
        field_144 = object["field_144"].GetFloat();
        field_148 = object["field_148"].GetFloat();
        field_14C = object["field_14C"].GetFloat();

        field_150 = object["field_150"].GetFloat();
        field_154 = object["field_154"].GetFloat();
        field_158 = object["field_158"].GetFloat();
        field_15C = object["field_15C"].GetFloat();
    }


    void ptxDomainVortex::WriteToJson(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();
        {
            WriteToJsonBase(writer);

            writer.String("field_130");
            writer.Double((double)field_130);
            writer.String("field_134");
            writer.Double((double)field_134);
            writer.String("field_138");
            writer.Double((double)field_138);
            writer.String("field_13C");
            writer.Double((double)field_13C);
        }
        writer.EndObject();
    }

    void ptxDomainVortex::LoadFromJson(rapidjson::GenericObject<true, rapidjson::Value>& object)
    {
        LoadFromJsonBase(object);

        field_130 = object["field_130"].GetFloat();
        field_134 = object["field_134"].GetFloat();
        field_138 = object["field_138"].GetFloat();
        field_13C = object["field_13C"].GetFloat();
    }
}