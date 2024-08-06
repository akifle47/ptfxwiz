#include "PtxDomain.h"

namespace rage
{
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
}