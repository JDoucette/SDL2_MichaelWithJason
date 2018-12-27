// ========================================================================
inline uint32_t
swizzle32( uint32_t n ) // Big Endian <-> Little Endian
{
    return 0
    | ((n >> 24) & 0x000000FF)
    | ((n >>  8) & 0x0000FF00)
    | ((n <<  8) & 0x00FF0000)
    | ((n << 24) & 0xFF000000);
}


// ========================================================================
inline uint8_t
mem_u8( const uint8_t *buffer )
{
   return buffer[0];
}


// ========================================================================
inline uint16_t
mem_u16( const uint8_t *buffer )
{
   uint16_t n = *(const uint16_t*)buffer;
   return n;
}


// ========================================================================
inline uint32_t
mem_u32( const uint8_t *buffer )
{
   uint32_t n = *(const uint32_t*)buffer;
   return n;
}


// ========================================================================
inline uint32_t
mem_u32be( const uint8_t *buffer )
{
   const uint32_t n = *(const uint32_t*)buffer;
   return swizzle32( n );
}


// ========================================================================
inline uint64_t
mem_u64( const uint8_t *buffer )
{
   const uint64_t n = *(const uint64_t*)buffer;
   return n;
}

// ________________________________________________________________________

// Unserialize Memory -> Native Types
// These update the pointer to the buffer

// ========================================================================
inline void
MemInt8( const uint8_t **buffer, uint8_t *n_ )
{
    *n_ = mem_u8( *buffer );
    *buffer += 1;
}

// ========================================================================
inline void
MemInt16( const uint8_t **buffer, uint16_t *n_ )
{
    *n_ = mem_u16( *buffer );
    *buffer += 2;
}


// ========================================================================
inline void
MemInt32( const uint8_t **buffer, uint32_t *n_ )
{
    *n_ = mem_u32( *buffer );
    *buffer += 4;
}


// C# BinaryReader.ReadSingle()
// https://msdn.microsoft.com/en-us/library/system.io.binaryreader.readsingle%28v=vs.110%29.aspx
// ========================================================================
inline void
MemReal32( const uint8_t **buffer, float *n_ )
{
    union
    {
        float    f;
        uint32_t i;
    } int_float;

    int_float.i = mem_u32( *buffer );
    *n_ = int_float.f;
    *buffer += 4;
}


// C# BinaryReader.ReadDouble()
// https://msdn.microsoft.com/en-us/library/system.io.binaryreader.readdouble%28v=vs.110%29.aspx
// ========================================================================
inline void
MemReal64( const uint8_t **buffer, double *n_ )
{
    union
    {
        double   f;
        uint64_t i;
    } int_float;

    int_float.i = mem_u64( *buffer );
    *n_ = int_float.f;
    *buffer += 8;
}
