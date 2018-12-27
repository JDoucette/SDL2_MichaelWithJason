// = Macros =
#ifndef     MATH_PI // WIN32:MSVC
    #define MATH_PI           3.1415962f
#endif   // MATH_PI
    #define DEG_TO_RAD(angle) (angle * MATH_PI / 180.f)

// @param alignment must be a power of 2
// @example: ALIGN(x,4)
// @reference - http://stackoverflow.com/questions/13122846/align-macro-kernel
#define ALIGN(x,alignment) ((x + alignment - 1) & ~(alignment-1))


inline float
clamp( const float x, const float minX, const float maxX )
{
    if( x < minX ) return minX;
    if( x > maxX ) return maxX;
    /*          */ return x;
}

inline float
invsqrtf( const float & x )
{
    /* */
    return (1.f / sqrtf( x ));
}

inline float
min( const float a, const float b )
{
    return (a < b) ? a : b;
}

inline int
max( int a, int b )
{
    return (a > b) ? a : b;
}

/*
// TODO; FIXME: CLEANUP: Not used anymore -- see SHADER_JET
Vector3f ColorJet( int y )
{
    Vector3f rgb;
    float k = 4.f * (y / 255.f);
    rgb.x = clamp( min( k - 1.5f, -k + 4.5f), 0.f, 1.f );
    rgb.y = clamp( min( k - 0.5f, -k + 3.5f), 0.f, 1.f );
    rgb.z = clamp( min( k + 0.5f, -k + 2.5f), 0.f, 1.f );
    return rgb;
}
*/


// y = log2( n )
// return y+1;
// i.e.
// 2^11 < 3000 < 2^12 = 4096
// return 12
inline uint32_t
log2_ceil( uint32_t x )
{
    uint32_t y = 1;
    while( (x >>= 1) > 0 )
        y++;
    return y;
}


inline uint32_t
log2_floor( uint32_t x )
{
    uint32_t y = 0;
    while( (x >>= 1) > 0 )
        y++;
    return y;
}

// http://stackoverflow.com/questions/2589096/find-most-significant-bit-left-most-that-is-set-in-a-bit-array
// Count Leading Zeroes
// GCC: __builtin_clz    https://gcc.gnu.org/onlinedocs/gcc-4.3.4/gcc/Other-Builtins.html
// MSVC: _BitScanReverse https://msdn.microsoft.com/en-us/library/fbxyd7zd(v=VS.80).aspx
inline uint32_t
msb32( uint32_t x )
{
    static const uint8_t bits[16] = {0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4};

    unsigned int r = 0;
    if (x & 0xFFFF0000) { r += 16/1; x >>= 16/1; }
    if (x & 0x0000FF00) { r += 16/2; x >>= 16/2; }
    if (x & 0x000000F0) { r += 16/4; x >>= 16/4; }
    return r + bits[x];
}


/** @note 0 will return incorrect -1
*/
// http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
// https://groups.google.com/forum/#!topic/comp.lang.python/xNOq4N-RffU
// http://stackoverflow.com/questions/466204/rounding-up-to-nearest-power-of-2
// http://stackoverflow.com/questions/1322510/given-an-integer-how-do-i-find-the-next-largest-power-of-two-using-bit-twiddlin
inline uint32_t
pow2_ceil( uint32_t x )
{
    // bsr = Bit Scan Reverse - return highest bit set
    //float    f = (float)x;
    //uint32_t i = *(uint32_t *) &f;
    //uint32_t s = 1U << ((i >> 23) - 0x7F);
    //return s << (i <x );

    //uint32_t y = 1;
    //while( x > 0 )
    //{
    //    x >>= 1;
    //    y <<= 1;
    //}
    //return y;

    x--;
    x |= x >>  1;
    x |= x >>  2;
    x |= x >>  4;
    x |= x >>  8;
    x |= x >> 16;
    x++;

    return x;
}

