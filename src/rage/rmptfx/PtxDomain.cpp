#include "PtxDomain.h"

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
}

uint32_t rage::ptxDomain::GetObjectSize() const
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