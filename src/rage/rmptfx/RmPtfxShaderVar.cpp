#include "RmPtfxShader.h"

namespace rage
{
    void rmPtfxShaderVar::Place(void* that, const datResource& rsc)
    {
        rsc.PointerFixUp(mName);

        switch(mType)
        {
            case eVarType::BOOL:
                new(that) rmPtfxShaderVar_Bool(rsc);
            break;

            case eVarType::INT:
                new(that) rmPtfxShaderVar_Int(rsc);
            break;

            case eVarType::FLOAT:
                new(that) rmPtfxShaderVar_Float(rsc);
            break;

            case eVarType::FLOAT2:
                new(that) rmPtfxShaderVar_Float2(rsc);
            break;

            case eVarType::FLOAT3:
                new(that) rmPtfxShaderVar_Float3(rsc);
            break;

            case eVarType::FLOAT4:
                new(that) rmPtfxShaderVar_Float4(rsc);
            break;

            case eVarType::TEXTURE:
                new(that) rmPtfxShaderVar_Texture(rsc);
            break;

            case eVarType::KEYFRAME:
                new(that) rmPtfxShaderVar_Keyframe(rsc);
            break;

            default:
                Log::Error("Invalid rmptfx shader var type - %d", mType);
        }
    }

    void rmPtfxShaderVar::AddToLayout(RSC5Layout& layout, uint32_t depth)
    {
        layout.AddObject(mName, RSC5Layout::eBlockType::VIRTUAL, strlen(mName) + 1);

        if(mType == eVarType::TEXTURE)
        {
            ((rmPtfxShaderVar_Texture*)this)->AddToLayout(layout, depth);
        }
        else if(mType == eVarType::KEYFRAME)
        {
            ((rmPtfxShaderVar_Keyframe*)this)->AddToLayout(layout, depth);
        }
    }

    void rmPtfxShaderVar::SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
    {
        layout.SerializePtr(mName, strlen(mName) + 1);

        if(mType == eVarType::TEXTURE)
        {
            ((rmPtfxShaderVar_Texture*)this)->SerializePtrs(layout, rsc, depth);
        }
        else if(mType == eVarType::KEYFRAME)
        {
            ((rmPtfxShaderVar_Keyframe*)this)->SerializePtrs(layout, rsc, depth);
        }
    }

    uint32_t rmPtfxShaderVar::GetObjectSize() const
    {
        switch(mType)
        {
            case eVarType::BOOL:
                return sizeof(rmPtfxShaderVar_Bool);
            break;

            case eVarType::INT:
                return sizeof(rmPtfxShaderVar_Int);
            break;

            case eVarType::FLOAT:
                return sizeof(rmPtfxShaderVar_Float);
            break;

            case eVarType::FLOAT2:
                return sizeof(rmPtfxShaderVar_Float2);
            break;

            case eVarType::FLOAT3:
                return sizeof(rmPtfxShaderVar_Float3);
            break;

            case eVarType::FLOAT4:
                return sizeof(rmPtfxShaderVar_Float4);
            break;

            case eVarType::TEXTURE:
                return sizeof(rmPtfxShaderVar_Texture);
            break;

            case eVarType::KEYFRAME:
                return sizeof(rmPtfxShaderVar_Keyframe);
            break;

            default:
                Log::Error("Invalid rmptfx shader var type - %d", mType);
                return 0;
        }
    }
}