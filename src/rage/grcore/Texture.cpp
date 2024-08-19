#include "Texture.h"

uint32_t rage::grcTexturePC::GetSize() const
{
    if(mLayerCount + 1 < 1)
    {
        return 0;
    }

    uint32_t size = 0;
    for(uint8_t layer = 0; layer < mLayerCount + 1; layer++)
    {
        for(uint8_t mip = 0; mip < mMipCount; mip++)
        {
            size += (mStride >> mip) * (mHeight >> mip);
        }
    }

    return size;
}

DDS_HEADER rage::grcTexturePC::GenerateDDSHeader() const
{
    DDS_HEADER header {};

    header.Width = mWidth;
    header.Height = mHeight;

    if(mLayerCount > 1 || mTextureType == eTextureType::VOLUME)
    {
        header.Depth = mLayerCount;
        header.Flags |= eDDSHeaderFlag::DEPTH;
        header.Caps2 |= eDDSHeaderCap2::VOLUME;
    }

    if(mMipCount > 1)
    {
        header.MipMapCount = mMipCount;
        header.Flags |= eDDSHeaderFlag::MIPMAPCOUNT;
        header.Caps  |= eDDSHeaderCap::COMPLEX | eDDSHeaderCap::MIPMAP;
    }

    if(mTextureType == eTextureType::CUBE)
    {
        header.Caps2 = eDDSHeaderCap2::CUBEMAP | eDDSHeaderCap2::CUBEMAP_FACES;
    }

    header.PixelFormat.FourCC = mFormat;
    header.PixelFormat.Flags |= eDDSPixelFormatFlag::FOURCC;

    return header;
}