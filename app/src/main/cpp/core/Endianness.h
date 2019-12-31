#pragma once

#pragma pack(push, 1)
#ifdef _MSC_VER
struct  ENDIANNESS
#else
struct __attribute__((packed)) ENDIANNESS
#endif // _MSC_VER
{
    enum VALUE
    {
        BIG_ENDIAN = 0,
        LITTLE_ENDIAN = 1,
    };

    inline static VALUE Get() { return data.bytes[3] == 0xFF ? BIG_ENDIAN : LITTLE_ENDIAN; };
    inline static bool IsBigEndian() { return data.bytes[3] == 0xFF; };
    inline static bool IsLittleEndian() { return data.bytes[3] == 0x00; };


    inline static unsigned int convert(unsigned int _value) { return _value >> 24 | ((_value >> 8) & 0x0000FF00) | ((_value << 8) & 0x00FF0000) | _value << 24; };
    inline static int convert(int _value) { return _value >> 24 | ((_value >> 8) & 0x0000FF00) | ((_value << 8) & 0x00FF0000) | _value << 24; };

    inline static unsigned short convert(unsigned short _value) { return _value >> 8 | _value << 8; };
    inline static short convert(short _value) { return _value >> 8 | _value << 8; };

private:
    constexpr static union
    {
        unsigned int full; // = 0x000000FF;
        unsigned char bytes[4];
    } data = {0x000000FF};
};
#pragma pack(pop)
