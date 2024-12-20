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

            if(rsc.Map->ContainsDest(&Ptr) == -1)
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
        }

        void SerializePtrs(RSC5Layout& layout, datResource& rsc, uint32_t depth)
        {
            if(!Ptr)
                return;

            layout.SerializePtr(Ptr, sizeof(uintptr_t));
        }

        inline void Place(void* that, const datResource& rsc)
        {
            new(that) datRef<T>(rsc);
        }

    private:
        T* Ptr;
    };
}