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
}