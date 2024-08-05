#pragma once

using DWORD = unsigned long;

inline constexpr DWORD DDS_MAGIC = 0x20534444;

enum class eDDSHeaderFlag : DWORD
{
     NONE        = 0x0,
     CAPS        = 0x1,
     HEIGHT      = 0x2,
     WIDTH       = 0x4,
     PITCH       = 0x8,
     PIXELFORMAT = 0x1000,
     MIPMAPCOUNT = 0x20000,
     LINEARSIZE  = 0x80000,
     DEPTH       = 0x800000
};
inline eDDSHeaderFlag operator|(eDDSHeaderFlag lhs, eDDSHeaderFlag rhs)
{
    return eDDSHeaderFlag(DWORD(lhs) | DWORD(rhs));
}
inline eDDSHeaderFlag operator|=(eDDSHeaderFlag& lhs, eDDSHeaderFlag rhs)
{
    lhs = eDDSHeaderFlag(DWORD(lhs) | DWORD(rhs));
    return lhs;
}

enum class eDDSHeaderCap : DWORD
{
    NONE    = 0x0,
    COMPLEX = 0x8,
    MIPMAP  = 0x400000,
    TEXTURE = 0x1000
};
inline eDDSHeaderCap operator|(eDDSHeaderCap lhs, eDDSHeaderCap rhs)
{
    return eDDSHeaderCap(DWORD(lhs) | DWORD(rhs));
}
inline eDDSHeaderCap operator|=(eDDSHeaderCap& lhs, eDDSHeaderCap rhs)
{
    lhs = eDDSHeaderCap(DWORD(lhs) | DWORD(rhs));
    return lhs;
}

enum class eDDSHeaderCap2 : DWORD
{
    NONE              = 0x0,
    CUBEMAP           = 0x200,
    CUBEMAP_POSITIVEX = 0x400,
    CUBEMAP_NEGATIVEX = 0x800,
    CUBEMAP_POSITIVEY = 0x1000,
    CUBEMAP_NEGATIVEY = 0x2000,
    CUBEMAP_POSITIVEZ = 0x4000,
    CUBEMAP_NEGATIVEZ = 0x8000,
    CUBEMAP_FACES     = CUBEMAP_POSITIVEX | CUBEMAP_NEGATIVEX | CUBEMAP_POSITIVEY | CUBEMAP_NEGATIVEY | CUBEMAP_POSITIVEZ | CUBEMAP_NEGATIVEZ,
    VOLUME            = 0x200000
};
inline eDDSHeaderCap2 operator|(eDDSHeaderCap2 lhs, eDDSHeaderCap2 rhs)
{
    return eDDSHeaderCap2(DWORD(lhs) | DWORD(rhs));
}
inline eDDSHeaderCap2 operator|=(eDDSHeaderCap2& lhs, eDDSHeaderCap2 rhs)
{
    lhs = eDDSHeaderCap2(DWORD(lhs) | DWORD(rhs));
    return lhs;
}

enum class eDDSPixelFormatFlag : DWORD
{
    NONE        = 0x0,
    ALPHAPIXELS = 0x1,
    ALPHA       = 0x2,
    FOURCC      = 0x4,
    RGB         = 0x40,
    YUV         = 0x200,
    LUMINANCE   = 0x20000
};
inline eDDSPixelFormatFlag operator|(eDDSPixelFormatFlag lhs, eDDSPixelFormatFlag rhs)
{
    return eDDSPixelFormatFlag(DWORD(lhs) | DWORD(rhs));
}
inline eDDSPixelFormatFlag operator|=(eDDSPixelFormatFlag& lhs, eDDSPixelFormatFlag rhs)
{
    lhs = eDDSPixelFormatFlag(DWORD(lhs) | DWORD(rhs));
    return lhs;
}

struct DDS_PIXELFORMAT 
{
    DDS_PIXELFORMAT()
    {
        Size = 32;

        Flags = eDDSPixelFormatFlag::NONE;
        FourCC = 0;

        RGBBitCount = 0;
        RBitMask = 0;
        GBitMask = 0;
        BBitMask = 0;
        ABitMask = 0;
    }

    DWORD Size;
    eDDSPixelFormatFlag Flags;
    DWORD FourCC;
    DWORD RGBBitCount;
    DWORD RBitMask;
    DWORD GBitMask;
    DWORD BBitMask;
    DWORD ABitMask;
};

struct DDS_HEADER
{
    DDS_HEADER() : Reserved1()
    {
        Size = 124;

        Flags = eDDSHeaderFlag::CAPS | eDDSHeaderFlag::HEIGHT | eDDSHeaderFlag::WIDTH | eDDSHeaderFlag::PIXELFORMAT;

        Height = 0;
        Width  = 0;
        PitchOrLinearSize = 0;
        Depth = 0;
        MipMapCount = 0;

        Caps      = eDDSHeaderCap::TEXTURE;
        Caps2     = eDDSHeaderCap2::NONE;
        Caps3     = 0;
        Caps4     = 0;
        Reserved2 = 0;
    }

    DWORD           Size;
    eDDSHeaderFlag  Flags;
    DWORD           Height;
    DWORD           Width;
    DWORD           PitchOrLinearSize;
    DWORD           Depth;
    DWORD           MipMapCount;
    DWORD           Reserved1[11];
    DDS_PIXELFORMAT PixelFormat;
    eDDSHeaderCap   Caps;
    eDDSHeaderCap2  Caps2;
    DWORD           Caps3;
    DWORD           Caps4;
    DWORD           Reserved2;
};