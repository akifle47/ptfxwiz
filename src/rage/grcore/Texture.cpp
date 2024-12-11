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

    header.Depth = mLayerCount + 1;
    if(mLayerCount > 1 || mTextureType == eTextureType::VOLUME)
    {
        header.Depth = mLayerCount + 1;
        header.Flags |= eDDSHeaderFlag::DEPTH;
        header.Caps2 |= eDDSHeaderCap2::VOLUME;
    }

    if(mMipCount >= 1)
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

void rage::grcTexturePC::LoadFromDDS(const DDS_HEADER& header, const void* pixelData)
{
    if(header.Flags & eDDSHeaderFlag::DEPTH)
        mLayerCount = (uint8_t)header.Depth - 1;
    else
        mLayerCount = 0;

    if(header.Flags & eDDSHeaderFlag::MIPMAPCOUNT)
        mMipCount = (uint8_t)header.MipMapCount;
    else
        mMipCount = 1;

    mWidth = (uint16_t)header.Width;
    mHeight = (uint16_t)header.Height;
    mFormat = (D3DFORMAT)header.PixelFormat.FourCC;

    if(mFormat == D3DFMT_DXT1)
        mStride = mWidth / 2;
    else if(mFormat == D3DFMT_DXT5 || mFormat == D3DFMT_DXT4 || mFormat == D3DFMT_DXT3 || mFormat == D3DFMT_DXT2)
        mStride = mWidth;
    else
    {
        Log::Error("grcTexturePC::LoadFromDDS: Unsupported texture format: %d.", (uint32_t)mFormat);
        return;
    }

    mPixelData = (void*)new uint8_t[GetSize()];
    memcpy(mPixelData, pixelData, GetSize());
}