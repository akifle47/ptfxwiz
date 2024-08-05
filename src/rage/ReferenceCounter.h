#pragma once
#include "../Utils.h"
#include "Base.h"

namespace rage
{
    class atReferenceCounter : public datBase
    {
        virtual void AddRef()
        {
            ++mRefCount;
        }

        virtual int32_t Release()
        {
            if(mRefCount-- == 1)
            {
                delete this;
                return 0;
            }

            return mRefCount;
        }

    private:
        int32_t mRefCount = 0;

    protected:
        ~atReferenceCounter() {};
    };
    ASSERT_SIZE(atReferenceCounter, 0x8);
}