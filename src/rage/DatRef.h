#pragma once
#include "Resource.h"

namespace rage
{
    template<typename T>
    struct datRef
    {
        datRef()
        {}

        datRef(T* ptr) : Ptr(ptr)
        {}

        datRef(const datResource& rsc)
        {
            if(!Ptr)
            {
                return;
            }

            if(rsc.Map->ContainsDest(Ptr) == -1)
            {
                Ptr = nullptr;
                return;
            }

            rsc.PointerFixUp(Ptr);
        }

        T& operator*() const 
        {
            return *Ptr; 
        }

        T* operator->() const 
        {
            return Ptr; 
        }

        T*& Get()
        {
            return Ptr;
        }

        void AddToLayout(RSC5Layout& layout, uint32_t depth)
        {
            if(!Ptr)
                return;

            layout.AddObject(Ptr, RSC5Layout::eBlockType::VIRTUAL);

            if constexpr(requires { Ptr->AddToLayout(layout, depth); })
            {
                Ptr->AddToLayout(layout, depth + 1);
            }
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            if(!Ptr)
                return;

            if constexpr(requires { Ptr->SerializePtrs(layout, rsc, depth); })
            {
                Ptr->SerializePtrs(layout, rsc, depth + 1);
            }

            layout.SerializePtr(Ptr, sizeof(T));
        }

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) datRef<T>(rsc);
        }

    private:
        T* Ptr;
    };
}