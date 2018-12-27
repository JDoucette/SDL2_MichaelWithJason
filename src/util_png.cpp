    // STB Image
    #define STBI_NO_JPEG  1
    //      STBI_NO_PNG
    #define STBI_NO_BMP   1
    #define STBI_NO_PSD   1
    #define STBI_NO_TGA   1
    #define STBI_NO_GIF   1
    #define STBI_NO_HDR   1
    #define STBI_NO_PIC   1
    #define STBI_NO_PNM   1
    #define STBI_NO_STDIO 1
    // https://github.com/nothings/stb/blob/master/stb_image.h
    #define STB_IMAGE_IMPLEMENTATION 
    #include "stb_image.h"


// ========================================================================
bool PNG_VerifyHeader( const size_t size, const uint8_t *buffer )
{
    const int                 HEADER_SIZE = 8;
    const uint8_t PNG_HEADER[ HEADER_SIZE + 1 ] = "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A"; // "\x89PNG\n\r\x1A\r"

    if( size < HEADER_SIZE )
        return false;

    for( int i = 0; i < HEADER_SIZE; i++ )
        if( buffer[i] != PNG_HEADER[i] )
            return false;

    return true;
}

// ========================================================================
bool PNG_GetInfo( const size_t size, const uint8_t *buffer, ImageInfo_t *info_ )
{
    bool  bValid = false;

    const uint8_t *pSrc = buffer + 8   ; // skip 8 byte header
    const uint8_t *pEnd = buffer + size;

    bValid = PNG_VerifyHeader( size, buffer );
    if( !bValid )
        return bValid;
    if( !info_ )
        return bValid;

    info_->clear();

    // http://www.libpng.org/pub/png/spec/1.2/PNG-Chunks.html
    struct PNG_IHDR
    {
        uint32_t width;
        uint32_t height;
        uint8_t  bitsperpixel;
        uint8_t  type;
        uint8_t  compression;
        uint8_t  filter;
        uint8_t  interlace;
    };

    struct PNG_CHUNK
    {
        uint32_t nSize;
        uint32_t nType;

        char     sType[8];

        const uint8_t* Get( const uint8_t *buffer )
        {
            nSize = mem_u32be( buffer + 0 );
            nType = mem_u32be( buffer + 4 );

            sType[0] = buffer[4];
            sType[1] = buffer[5];
            sType[2] = buffer[6];
            sType[3] = buffer[7];
            sType[4] = 0;

            return buffer + 8;
        }
    };

    PNG_IHDR  tInfo;
    PNG_CHUNK chunk;

    while( pSrc < pEnd )
    {
        const uint8_t *pData = chunk.Get( pSrc );

//char text[ 256 ];
//sprintf( text, "Size: %08X   \n", chunk.nSize );
//TRACE( text );
//sprintf( text, "Type: %08X %s\n", chunk.nType, chunk.sType );
//TRACE( text );

        if( chunk.nType == 0x49484452 ) // IHDR
        {
            tInfo         = *(PNG_IHDR*)pData;
            tInfo.width   = swizzle32( tInfo.width  );
            tInfo.height  = swizzle32( tInfo.height );

            info_->nWidth        = (uint16_t) mem_u32be( pData + 0 );
            info_->nHeight       = (uint16_t) mem_u32be( pData + 4 );
            info_->nBitsPerPixel = (uint16_t) mem_u8   ( pData + 8 );

            /*
                http://www.w3.org/TR/PNG/#11IHDR
                http://www.w3.org/TR/PNG-Chunks.html

                Type    Bit Depths  Meaning
                0       1,2,4,8,16  Each pixel is a grayscale sample.
                2       8,16        Each pixel is an R,G,B triple.
                3       1,2,4,8     Each pixel is a palette index; a PLTE chunk must appear.
                4       8,16        Each pixel is a grayscale sample, followed by an alpha sample.
                6       8,16        Each pixel is an R,G,B triple, followed by an alpha sample.
            */
            if     ( tInfo.type == 2 ) { info_->nFormat = GL_RGB ; info_->nBitsPerPixel *= 3; } // 3 channels
            else if( tInfo.type == 6 ) { info_->nFormat = GL_RGBA; info_->nBitsPerPixel *= 4; } // 4 channels

            info_->nBytes        = ((info_->nWidth * info_->nHeight) * info_->nBitsPerPixel) >> 3;
        }
        // 0x73524742 sRGB
        // 0x67414D41 gAMA
        // 0x504C5445 PLTE
        else
        if( chunk.nType == 0x49444154 ) // IDAT
        {
            // Reference:
            // https://github.com/nothings/stb/blob/master/stb_image.h
            //    stbi__parse_png_file()
            // https://www.libsdl.org/projects/SDL_image/
            //    Requres lib:  SDL2_image.dll and zlib
            // http://www.libpng.org/pub/png/libpng.html
            info_->pTexels = (uint8_t*)pSrc; // TODO: Need to decompress! "At present, only compression method 0 (deflate/inflate compression with a sliding window of at most 32768 bytes) is defined."

            bValid = true;
        }
        // 0x49454E44 IEND

        pSrc += 8; // size, type
        pSrc += chunk.nSize;
        pSrc += 4; // CRC32
    }

    return bValid;
}


/** Will fill buffer (texels_) with pixel data
    @returns bool TRUE if was able to read the PNG
*/
// ========================================================================
bool PNG_Load( const char *filename, ImageInfo_t *info_, uint8_t *pixels_ = 0 )
{
    bool   bValid = false;
    size_t nSize  = 0;

    if (!filename)
    {
#if DEBUG
        Game_Fatal( "PNG_Load() NULL filename!" );
#endif
        return bValid;
    }

    if( !pixels_ )
    {
#if DEBUG
        Game_Fatal( "PNG_Load() NULL texels!" );
#endif
        return bValid;
    }

    FILE *pFile = File_OpenGetSize( filename, &nSize );
    if( pFile && nSize )
    {
        // Use temp 1MB mem pool for reading
        if( nSize > _1M )
        {
            Game_Fatal( "PNG > 1MB" );
            File_Close( pFile );
            return bValid;
        }

        int w;
        int h;
        int n; // bytes per pixel
        uint8_t *p = Mem_Alloc( _1M );

            // slurp entire file into temp memory buffer
            File_Read( pFile, p, nSize );
            PNG_GetInfo( nSize, p, info_ );

            unsigned char *pTexels = stbi_load_from_memory( p, (int)nSize, &w, &h, &n, STBI_rgb_alpha );

            info_->pTexels = pixels_;
            info_->nBytes  = w * h * n;
#if DEBUG
    TRACE( "   PNG: w: %d  h:%d   FileSize: %u  TexSize: %u  tex: %p\n", w, h, (uint32_t)nSize, info_->nBytes, pTexels );
#endif
            memcpy( pixels_, pTexels, info_->nBytes ); // dst, src, len
            stbi_image_free( pTexels );

        Mem_Deloc();
    }
    File_Close( pFile );

    return bValid;
}


/** Allocates a memory block and fills in image info
    @returns bool - TRUE if was able to read the PNG
    Uses 2 MemPools - returns the second one which has the texels
*/
// ========================================================================
bool PNG_Load_MemAlloc( const char *filename, ImageInfo_t *info_, MemPoolBlockData_t *mem_ )
{
    bool bValid = false;
    size_t nSize  = 0;

    if( !filename
    ||  !info_
    ||  !mem_
    )
    {
#if DEBUG
        Game_Fatal( "PNG_Load() BAD filename, image info, memory!" );
#endif
        return bValid;
    }

    // Don't need to call info_->clear() since PNG_GetInfo() will

    uint8_t *buffer= Mem_Alloc( _1M );
        FILE *pFile = File_OpenGetSize( filename, &nSize );
            if( pFile && nSize )
            {
                bValid = (nSize < _1M);
                if( !bValid ) { Game_Fatal( "PNG > 1M" ); goto bail; }

                File_Read( pFile, buffer, nSize );

                bValid = PNG_GetInfo( nSize, buffer, info_ );
                if( !bValid ) { Game_Fatal( "PNG bad header!" ); goto bail; }

                info_->pTexels = Mem_Alloc( info_->nBytes, mem_ );

                int w;
                int h;
                int n; // bytes per pixel
                unsigned char *pTexels = stbi_load_from_memory( buffer, (int)nSize, &w, &h, &n, STBI_rgb_alpha );
                    memcpy( info_->pTexels, pTexels, info_->nBytes ); // dst, src, len
                stbi_image_free( pTexels );
            }
bail:
        File_Close( pFile );
    Mem_Deloc();

    return bValid;
}
