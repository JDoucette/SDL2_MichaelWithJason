// From Memory
// ========================================================================
bool TGA_GetInfo( const size_t size, uint8_t *buffer, ImageInfo_t *info_ )
{
    if( !info_ )
        return false;

    info_->clear();

    if( size < 18 ) // missing header
        return false;

    if( buffer[2] != 2 ) // uncompressed RGB
        return false;

    // https://en.wikipedia.org/wiki/Truevision_TGA
    info_->nWidth        = (buffer[13] << 8) | (buffer[12] << 0);
    info_->nHeight       = (buffer[15] << 8) | (buffer[14] << 0);
    info_->nBitsPerPixel =                     (buffer[16] << 0);
    info_->pTexels       =  buffer + 18;

    bool bUpsideDown     = (buffer[17] & 0x20) ? false : true; // Origin lower left is normal for OpenGL
    if(  bUpsideDown )
        info_->bFlags |= ImageInfo_t::FLAG_UPSIDE_DOWN;

    info_->nBytes = ((info_->nWidth * info_->nHeight) * info_->nBitsPerPixel) >> 3;
    if( info_->nBytes > size )
        return false;

    // TODO: TGA has palette??

#if 0 // _DEBUG
    char text[ 256 ];
    sprintf( text, "H Len: %d     \n", buffer[0] );
    OutputDebugStringA( text );
    sprintf( text, "palet: %d     \n", buffer[1] );
    OutputDebugStringA( text );

    sprintf( text, "Size : %08X   \n", (uint32_t) size );
    OutputDebugStringA( text );
    sprintf( text, "Bytes: %08X   \n", (uint32_t) bytes );
    OutputDebugStringA( text );
#endif

    return true;
}


/** Read TGA from disk, fill in image stats and texels
    @param {String}      filename full path to read
    @param {ImageInfo_t} info_    image dimensions and stats
    @param {byte*}       texels_  raw buffer of texels to write to
    @note  texels_ buffer MUST be large enough!
*/
// ========================================================================
bool TGA_Load( const char *filename, ImageInfo_t *info_, uint8_t *texels_ = 0, int format = GL_BGRA )
{
    bool   bValid = false;
    size_t nSize  = 0;

    if( (!filename)
    ||  (!info_)
    ||  (!texels_)
    )   return bValid;

    FILE *pFile = File_OpenGetSize( filename, &nSize );
    if( pFile && nSize )
    {
        uint8_t aHeader[18];
        File_Read( pFile, aHeader, 18 );

        // GetInfo() sets the texels past the buffer but we ignore that since the caller already gave us a texel buffer
        bValid = TGA_GetInfo( nSize, &aHeader[0], info_ );

//char text[ 256 ];
//sprintf( text, "Size : %08X   \n", (uint32_t) nSize );
//OutputDebugStringA( text );
//sprintf( text, "Width  %d\n", (uint32_t) *width_ );
//OutputDebugStringA( text );
//sprintf( text, "Height %d\n", (uint32_t) *height_ );
//OutputDebugStringA( text );

        const int w = info_->nWidth ;
        const int h = info_->nHeight;

        if( bValid )
        {
            File_Read( pFile, texels_, nSize - 18 );

            // 24 -> 3 bytes_per_pixel
            // 32 -> 4 bytes_per pixel
            const size_t  nBytesPerPixel   = info_->nBitsPerPixel >> 3;
            const size_t  nBytePerScanline = w * nBytesPerPixel;

            /* */ uint8_t temp;
            /* */ uint8_t *pSrc = texels_;
            /* */ uint8_t *pDst = texels_ + info_->nBytes - nBytePerScanline;

            if( info_->bFlags & ImageInfo_t::FLAG_UPSIDE_DOWN )
            {
                // Swap rows [0] and [y-1], etc
                for( int y = 0; y < h/2; y++ )
                {
                    for( unsigned x = 0; x < nBytePerScanline; x++ )
                    {
                        temp = *pSrc;               
                               *pSrc = *pDst;       
                                       *pDst = temp;
                        pSrc++;
                        pDst++;
                    }
                    pDst -= 2 * nBytePerScanline;
                }
            }

            // Disk: B,G,R,A
            // GL_BGRA, no swizzle needed
            // GL_RGBA swizzle BGRA -> RGBA
            pSrc = texels_;

            if( (format != GL_BGRA) && (info_->nBitsPerPixel == 32) )
                for( uint32_t i = 0; i < info_->nBytes; i++, pSrc += 4 )
                {
                    temp = pSrc[0];                  // B
                           pSrc[0] = pSrc[2];        // B<-R
                                     pSrc[2] = temp; //    R
            }
        }
    }
    File_Close( pFile );

    return bValid;
}


/** Read TGA from disk, fill in image stats and texels
    @param {String}             filename full path to read
    @param {ImageInfo_t}        info_    image dimensions and stats
    @param {MemPoolBlockData_t} mem_     pick the appropiate mem pool, block handle, and allock memory
*/
// ========================================================================
bool TGA_Load_MemAlloc( const char *filename, ImageInfo_t *info_, MemPoolBlockData_t *mem_ )
{
    bool   bValid = false;
    size_t nSize  = 0;

    if( (!filename)
    ||  (!info_)
    ||  (!mem_)
    )   return bValid;

    FILE *pFile = File_OpenGetSize( filename, &nSize );
    if( pFile && nSize )
    {
        // Use temp mem pool - 1MB for reading
        if( nSize > _1M )
        {
            File_Close( pFile );
            return bValid;
        }

        uint8_t *p = Mem_Alloc( _1M );
            // slurp entire file into memory buffer
            File_Read( pFile, p, nSize );

            TGA_GetInfo( nSize, p, info_ );
//          uint8_t *pTexels = Mem_Alloc( info_->nBytes, mem_ ); // intentional alloc mem and return to caller
//            memcpy( pTexels, info_->pTexels, info_->nBytes );
        Mem_Deloc();
    }
    File_Close( pFile );


    return bValid;
}


// ========================================================================
bool TGA_Save( const char *filename, const int width, const int height, const void *texels, const int bitsPerPixel )
{
    bool bValid = false;

    if( (!filename)
    ||  (!texels)
    ||  (!width)
    ||  (!height)
    ||  (!bitsPerPixel)
    )   return bValid;

    if( (bitsPerPixel == 24) || (bitsPerPixel == 32) )
    {
        FILE *pFile = fopen( filename, "wb" );
        if( pFile )
        {
            uint8_t header[18];
            memset( header, 0, 18 );

            header[ 2] = (uint8_t) 2; // uncompressed RGB
            header[12] = (uint8_t) (width  & 255);
            header[13] = (uint8_t) (width  >>  8);
            header[14] = (uint8_t) (height & 255);
            header[15] = (uint8_t) (height >>  8);
            header[16] = (uint8_t) (bitsPerPixel);
            header[17] = (uint8_t) 0x20; // origin: bottom left

            const size_t area  = (width * height);
            const size_t bytes = (area *  bitsPerPixel) >> 3;

            if( fwrite( header, 18, 1, pFile) )
                if( fwrite( texels, bytes, 1, pFile) )
                    bValid = true;

            fclose( pFile );
        }
    }

    return bValid;
}
