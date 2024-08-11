#include "Shader.h"

namespace rage
{
    void grmShader::Place(void* that, const datResource& rsc)
    {
        new(that) grmShaderFx(rsc);
    }
}