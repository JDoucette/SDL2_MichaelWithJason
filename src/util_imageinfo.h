// = Types =
    struct ImageInfo_t
    {
        enum ImageFlags_e {
            FLAG_UPSIDE_DOWN = (1 << 0)
        };

        uint8_t *pTexels       ; // 8
        uint16_t nWidth        ; // 2
        uint16_t nHeight       ; // 2
        uint32_t nBytes        ; // 4 -- size_t is 8 bytes, wastes 4 bytes
        uint32_t nFormat       ; // 4 GL_RGBA or GL_BGRA
        uint8_t  nBitsPerPixel ; // 1 -- should be 1 but to keep total size aligned 
        uint8_t  bFlags        ; // 1 -- UPSIDE_DOWN
        uint16_t nBytesPerRow  ; // 2
                                 //==
                                 //24 bytes
        void clear()
        {
            pTexels       = 0;
            nWidth        = 0;
            nHeight       = 0;
            nBytes        = 0;
            nFormat       = 0;
            nBitsPerPixel = 0;
            bFlags        = 0;
            nBytesPerRow  = 0;
        }

#if DEBUG
        void dump()
        {
            TRACE( "Image: %dx%d @ %p  bpp: %d  bytes: %u\n"
                , nWidth
                , nHeight
                , pTexels
                , nBitsPerPixel
                , nBytes
            );
        }
#endif
    };

    struct ImageRect_t
    {
        uint16_t x, y;
        uint16_t w, h;

    };

    template <typename T>
    bool
    PointInRect( const T& px, const T& py, const T& rx, const T& ry, const T& rw, const T& rh )
    {
#ifdef _MSC_VER
        // http://stackoverflow.com/questions/12042433/how-to-disable-c4127-for-do-whilefalse
        #pragma warning( push )
        #pragma warning( disable : 4127 )
#endif
        if( true  // stupid warning C4127: conditional expression is constant */
        && (px >= (rx     ))
        && (px <  (rx + rw))
        && (py >= (ry     ))
        && (py <  (ry + rh))
        )
            return true;

        return false;
#ifdef _MSC_VER
        #pragma warning( pop )
#endif
    }

    bool
    PointInRect16( const uint16_t x, const uint16_t y, const ImageRect_t& rect )
    {
        return PointInRect<uint16_t>( x, y, rect.x, rect.y, rect.w, rect.h );
    }

