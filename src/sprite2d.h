// = Types =

    // 360 degrees / 8 directions = 45 degrees/direction
    // CCW to match Math / Trigonometry
    enum FacingDir4_e
    {
         FACING_DIR4_E
        ,FACING_DIR4_N
        ,FACING_DIR4_W
        ,FACING_DIR4_S
        ,NUM_FACING_DIR4
    };

    enum FacingDir6_e
    {
         FACING_DIR6_E
        ,FACING_DIR6_N
        ,FACING_DIR6_W
        ,FACING_DIR6_S
        ,FACING_DIR6_U
        ,FACING_DIR6_D
        ,NUM_FACING_DIR6
    };

    // 360 degrees / 8 directions = 45 degrees/direction
    enum FacingDir8_e 
    {
         FACING_DIR8_E 
        ,FACING_DIR8_NE
        ,FACING_DIR8_N 
        ,FACING_DIR8_NW
        ,FACING_DIR8_W 
        ,FACING_DIR8_SW
        ,FACING_DIR8_S 
        ,FACING_DIR8_SE
        ,NUM_FACING_DIR8
    };

    // To determine which sub-tile for 9-slice
    enum NeighborMask8_e
    {
         NEIGHBOR_MASK8_E  = (1 << FACING_DIR8_E )
        ,NEIGHBOR_MASK8_NE = (1 << FACING_DIR8_NE)
        ,NEIGHBOR_MASK8_N  = (1 << FACING_DIR8_N )
        ,NEIGHBOR_MASK8_NW = (1 << FACING_DIR8_NW)
        ,NEIGHBOR_MASK8_W  = (1 << FACING_DIR8_W )
        ,NEIGHBOR_MASK8_SW = (1 << FACING_DIR8_SW)
        ,NEIGHBOR_MASK8_S  = (1 << FACING_DIR8_S )
        ,NEIGHBOR_MASK8_SE = (1 << FACING_DIR8_SE)
    };

// = Public =

// ========================================================================
Vector2f
ScreenSpaceToWorldSpace( Vector2f vLocationScreenSpace, Vector2f *pvCamera = NULL, float nWorldSpaceToImageSpaceScale = 16.f )
{
    Vector2f vLocationWorldSpace = vLocationScreenSpace;
    if( pvCamera )
    {
        vLocationWorldSpace.x -= (gnGameWidth  / 2);
        vLocationWorldSpace.y -= (gnGameHeight / 2);
        vLocationWorldSpace *= 1.f/nWorldSpaceToImageSpaceScale;
        vLocationWorldSpace += *pvCamera;
    }

    return vLocationWorldSpace;
}

// NOTE: 1m world space = 16 px screen space
Vector2f
WorldSpaceToScreenSpace( Vector2f vWorldSpace, Vector2f *pvCamera = NULL, float nWorldSpaceToImageSpaceScale = 16.f )
{
    Vector2f vLocationScreenSpace = vWorldSpace;

    if( pvCamera )
    {
        vLocationScreenSpace -= *pvCamera;
        vLocationScreenSpace *=  nWorldSpaceToImageSpaceScale;

        vLocationScreenSpace.x += (gnGameWidth  / 2);
        vLocationScreenSpace.y += (gnGameHeight / 2);
    }

    // Floaty sprites is not a bug, but a lack of precision in level rendering -- tiles are snapped to every 16 px
    // We can still end up with +/- 1 px sprite jitter  due to the camera not being quantized to the pixel grid
    return vLocationScreenSpace;
}

    struct SpriteBuffer_t
    {
        float   *pXY; // TODO: FIXME: uint16_t
        float   *pUV; // TODO: FIXME: uint16_t 12.4 fixed point
        uint8_t  len; // glyphs rendered
        int      nXY; // total num of xy
        int      nUV; // total num of uv
        int      sXY; // total size of xy
        int      sUV; // total size of uv
        float    rgba[4];
    };

    struct SpriteRect // TODO: Shouldn't we use ImageRect_t ?
    {
        uint16_t x, y;
        uint16_t w, h;
    };

class Sprite2D
{
    // switched from "private" to "public" to test the "clone" method below...
    protected:
        enum SpriteFlags_e
        {
              SPRITE_FLAG_IS_ANIMATING     = (1 << 0)
            , SPRITE_FLAG_FLIP_X           = (1 << 1)
            , SPRITE_FLAG_AUTO_FACE_PLAYER = (1 << 2) // some enemies walk away from player
            , SPRITE_FLAG_FLIP_IS_REVERSED = (1 << 3)
            , SPRITE_FLAG_IS_CLONE         = (1 << 4)
            , SPRITE_FLAG_IS_HIDDEN        = (1 << 5)
        };

        uint16_t          _iFrame   ;
        uint16_t          _nFrame   ;
        SpriteRect        _aFrame[16];

        float             _aXY [4*3]; // TODO: OPTIMIZE: uint16
        float             _aUV [4*2]; // TODO: OPTIMIZE: uint16
        float             _rgba[4  ]; // vertex colors
        float             _scaleW   ;
        float             _scaleH   ;

        // Animation
        uint16_t           _bFlags  ;
        uint16_t           _iAnimFrameBegin;
        uint16_t           _iAnimFrameEnd  ;
        float              _nAnimTimer     ;
        float              _nAnimDuration  ;

        // Texels
        void*             _pData;

        uint16_t          _tw       ; // texture atlas w
        uint16_t          _th       ; // texture atlas h
        float             _ootw     ; // one over w
        float             _ooth     ; // one over h

        GLuint            _hTexture ; // handle Texels
        GLuint            _hvBuffer ; // xy     Vertex
        GLuint            _htBuffer ; // uv     Texture
        GLuint            _hkBuffer ; // rgba   Color


        // ========================================================================
        void initialize()
        {
            memset( this, 0, sizeof( *this ) );
            _scaleW      = 1.0f;
            _scaleH      = 1.0f;
            facingDir    = FACING_DIR4_E;
        }

        Vector2f _vLocationScreenSpace; // used to be x,y

    public:
        Vector2f _vDeltaImageSpace; // HACK: Exposed to game for testing/debuggin

        // Vec3 vLocation;
        // Vec3 vVelocity;
        // Vec3 vAcceleration;
        Vector2f vLocation; // WorldSpace (could be Image Space if Update() camera == NULL)

        float  speed;
        int8_t facingDir; // +1 = Right, -1 = Left

        // ========================================================================
#if DEBUG
        void debug()
        {
            int i;

            for( i = 0; i < 12; i+= 3 )
            {
                printf( "xy[%d]: ", i / 3 );
                printf( " %7.2f, %7.2f, %7.2f\n", _aXY[ i+0 ], _aXY[i+1], _aXY[i+2] );
            }

            for( i = 0; i < 8; i+= 2 )
            {
                printf( "uv[%d]: ", i / 2 );
                printf( " %1.5f, %1.5f\n", _aUV[ i+0 ], _aUV[i+1] );
            }
        }
#endif

        // Doesn't include extension
        // ========================================================================
        Sprite2D()
        {
            initialize();
        }

        // ========================================================================
        ~Sprite2D() {};

        // Adjusts the image space location so the character stands centered left/right in a tile in world space
        // ========================================================================
        void AlignCenterWorldSpace()
        {
            _vDeltaImageSpace.x = (getFrameWidth() *-0.5f);
        }

        // Problem: We want an item that sits on the floor to have the same y world space location
        // The problem is that when this sprite is rendered it will be drawn below the floor
        // The solution is to have a delta location in image space so the item is drawn at a higher location
        // so the bottom of the item rests on the ground
        //
        // Adjusts the image space location so the character stands above a tile in world space
        // ========================================================================
        void AlignFloorWorldSpace()
        {
            // HACK: this should be +2 from frame not +4 ... bug in ImageSpace ?
            _vDeltaImageSpace.y = (getFrameHeight() * -1.f) + 4.f; // NPCs have 2 pixel at bottom of frame!?!?!
        }

        // ========================================================================
        void
        Animate( bool isAnimating, float frameDuration = 1.f, int startFrame = 0, int endFrame = 0 )
        {
            SetAnimating( isAnimating );

            if( startFrame > _nFrame )
                startFrame = 0;
            if( endFrame   > _nFrame )
                endFrame   = _nFrame-1;

            _iFrame          = (uint16_t) startFrame;
            _iAnimFrameBegin = (uint16_t) startFrame;
            _iAnimFrameEnd   = (uint16_t) endFrame;
            _nAnimTimer      = 0.f;
            _nAnimDuration   = frameDuration;
        }

        /** @example:
            Load(); // or MakeTexture()
            BindTexture(); 
            // UploadTexture(); // optional: needed for dynamic texture
            Update( dTime );
            Render();
        */
        // ========================================================================
        void
        BindTexture( int format = GL_RGBA )
        {
            _ootw = 1.f / (float)_tw;
            _ooth = 1.f / (float)_th;

            // GL_TEXTURE_WRAP_S
            // GL_TEXTURE_WRAP_T
            // glTexImage2D
            _hTexture = OpenglCreateTexture(
                 _tw
                ,_th
                ,_pData
                ,format
            );

//          GLuint hTexture;
//          glGenTextures( 1, &hTexture );
//          glBindTexture( GL_TEXTURE_2D, hTexture );
//          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S    , GL_REPEAT ); // CLAMP_TO_EDGE );
//          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T    , GL_REPEAT ); // CLAMP_TO_EDGE );

            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

            _hvBuffer = OpenglCreateBuffer(); // xy
            _htBuffer = OpenglCreateBuffer(); // uv
            _hkBuffer = OpenglCreateBuffer(); // rgba
        }

        // ========================================================================
        void
        Clone( const Sprite2D * pSprite )
        {
            // Texels
            // reference the same pointer, BUT mark this as a clone so we don't try and free the parent sprite!
            _pData = pSprite->_pData;

            //
            // copy all of the member variables
            //
            _iFrame    = pSprite->_iFrame;
            _nFrame    = pSprite->_nFrame;

            memcpy( _aFrame, pSprite->_aFrame, sizeof( pSprite->_aFrame ) );
            memcpy( _aXY   , pSprite->_aXY   , sizeof( pSprite->_aXY    ) );
            memcpy( _aUV   , pSprite->_aUV   , sizeof( pSprite->_aUV    ) );
            memcpy( _rgba  , pSprite->_rgba  , sizeof( pSprite->_rgba   ) );
/*
            for(int iFrame = 0; iFrame < 16; iFrame++)
                _aFrame[iFrame] = pSprite->_aFrame[iFrame];

            for(int iXY = 0; iXY < 4*3; iXY++)
                _aXY[iXY] = pSprite->_aXY[iXY];

            for(int iUV = 0; iUV < 4*2; iUV++)
                _aUV[iUV] = pSprite->_aUV[iUV];

            for(int iRGBA = 0; iRGBA < 4; iRGBA++)
                _rgba[iRGBA] = pSprite->_rgba[iRGBA];
*/
            _scaleW             = pSprite->_scaleW;
            _scaleH             = pSprite->_scaleH;

            // Animation
            _bFlags             = pSprite->_bFlags;
            _iAnimFrameBegin    = pSprite->_iAnimFrameBegin;
            _iAnimFrameEnd      = pSprite->_iAnimFrameEnd;
            _nAnimTimer         = pSprite->_nAnimTimer;
            _nAnimDuration      = pSprite->_nAnimDuration;

            _tw                 = pSprite->_tw;
            _th                 = pSprite->_th;
            _ootw               = pSprite->_ootw;
            _ooth               = pSprite->_ooth;

            _hTexture           = pSprite->_hTexture;
            _hvBuffer           = pSprite->_hvBuffer;
            _htBuffer           = pSprite->_htBuffer;
            _hkBuffer           = pSprite->_hkBuffer;


            _bFlags            |= SPRITE_FLAG_IS_CLONE;
        }

        // ========================================================================
        void
        FlipX( bool flipped )
        {
            if( flipped ) _bFlags |=  SPRITE_FLAG_FLIP_X;
            else          _bFlags &= ~SPRITE_FLAG_FLIP_X;
        }

        // ========================================================================
        void
        Free()
        {
            // we don't free clones, we leave that to the parent, the clone is just a pointer to the parent's memory
            if(! IsClone() )
                free( _pData );
        }

        // ========================================================================
        int  getFrame(                 ) const { return _iFrame; }
        void setFrame( const int frame ) {  if( frame < _nFrame ) _iFrame = (uint16_t) frame; }

        // ========================================================================
        int  getFrameWidth () const { return _aFrame[ _iFrame ].w; }
        int  getFrameHeight() const { return _aFrame[ _iFrame ].h; }

        int  getFrameTilesWidth () const { return (getFrameWidth () + 15) / 16; }
        int  getFrameTilesHeight() const { return (getFrameHeight() + 15) / 16; }

        // Sprite Scaling
        float getScale() const        { return _scaleW;  }
        void  setScale( float scale ) { _scaleW = scale; _scaleH = scale; }

        void setScaleW( float scaleW ) { _scaleW = scaleW; }
        void setScaleH( float scaleH ) { _scaleH = scaleH; }

        int  getTextureWidth()  const { return _tw; }
        int  getTextureHeight() const { return _th; }

        void Hide()              { _bFlags |=  SPRITE_FLAG_IS_HIDDEN; }
        void Show()              { _bFlags &= ~SPRITE_FLAG_IS_HIDDEN; }

        bool IsHidden() { return ( _bFlags &   SPRITE_FLAG_IS_HIDDEN) ? true : false; }

        bool IsVisible() { return ! IsHidden(); }
        void SetVisible(bool isVisible)
        {
            if( isVisible ) Show();
            else            Hide();            
        }

        // ========================================================================
        bool
        IsClone() const
        {
            return _bFlags & SPRITE_FLAG_IS_CLONE ? true : false;
        }

        // For Dir_E call FlipX(  result )
        // For Dir-W call FlipX( !result )
        // ========================================================================
        bool IsFlipReversed()
        {
            return (_bFlags & SPRITE_FLAG_FLIP_IS_REVERSED) ? true : false;
        }

        // ========================================================================
        bool
        Load( const char *filename, const int framesHorz = 0, const int framesVert = 0 )
        {
            bool bValid = false;
            char path[ 256 ] = "";

            if( !filename )
                return bValid;

            // we must turn the clone OFF if we load the asset otherwise we won't free the memory that we just loaded
            _bFlags &= ~SPRITE_FLAG_IS_CLONE; // TODO: FIXME: HACK: Clones (probably?) shouldn't be loading data ...
            if( IsClone() )
            {
                TRACE( "INFO: Cloned sprite loading data...\n" );
            }

            size_t nSizeTextureAtlas = 0;
            bool   bPNG              = 0;

            int nLen = (int)strlen( filename );
            if( nLen > 4 )
                if( strcmp( filename + nLen - 4, ".png" ) == 0 )
                    bPNG = 1;

            // Couldnt' load from 'mod' directory, try default built in file path
            if( !nSizeTextureAtlas )
            {
                strcpy( path, GAME_PATH_DATA );
                strcat( path, filename       );
                nSizeTextureAtlas = File_Size( path );
            }

            // absolute path
            if( !nSizeTextureAtlas )
            {
                char text[ 256 ];
                sprintf( text, "SPRITE: Couldn't load file: %s", path );
                Game_Fatal( "FILE", text );
            }

#if 0 // DEBUG
    TRACE( "  SPRITE: %s FileSize: %u\n", filename, (uint32_t)nSizeTextureAtlas );
#endif

            ImageInfo_t info;
            uint16_t nFrameH = (uint16_t) framesHorz;
            uint16_t nFrameV = (uint16_t) framesVert;

            if( filename )
            {
                if( bPNG )
                    bValid = PNG_Load_MemAlloc( path, &info, &_pData );
            }

            if( bValid )
            {
// TODO: CLEANUP: make( info.nWidth, info.nWidth, 32, mem.pData );
                _tw = info.nWidth;
                _th = info.nHeight;

                _aFrame[0].x = 0;
                _aFrame[0].y = 0;
                _aFrame[0].w = _tw; // TileW is entire texture
                _aFrame[0].h = _th; // TileH is entire texture

                _nFrame = 1;
            }

            if ((framesHorz == 0) && (framesVert == 0)) // TODO: DOCUMENT
            {
            }
            else
            if ((framesHorz == 0) && (framesVert > 0)) // TODO: DOCUMENT
            {
                _nFrame = (uint16_t) framesVert;
                uint16_t frameW = _tw;
                uint16_t frameH = _th / nFrameV;
                uint16_t offset = 0;

                for( int iFrame = 0; iFrame < _nFrame; iFrame++, offset += frameH )
                {
                    _aFrame[ iFrame ].x = 0;
                    _aFrame[ iFrame ].y = offset;
                    _aFrame[ iFrame ].w = frameW;
                    _aFrame[ iFrame ].h = frameH;
                }
            }
            else
            if ((framesHorz > 0) && (framesVert == 0)) // TODO: DOCUMENT
            {
                _nFrame = (uint16_t) framesHorz;
                uint16_t frameW = _tw / nFrameH;
                uint16_t frameH = _th;
                uint16_t offset = 0;

                for( int iFrame = 0; iFrame < _nFrame; iFrame++, offset += frameW )
                {
                    _aFrame[ iFrame ].x = offset;
                    _aFrame[ iFrame ].y = 0;
                    _aFrame[ iFrame ].w = frameW;
                    _aFrame[ iFrame ].h = frameH;
                }
            }
            else
            if ((framesHorz > 0) && (framesVert > 0)) // glyphs same size
            {
                // all frames same size
                _aFrame[0].w = (uint16_t) framesHorz; // framesHorz is glyph width  in pixels
                _aFrame[0].h = (uint16_t) framesVert; // framesVert is glyph height in pixels
            }
            else
            if ((framesHorz < 0) && (framesVert < 0)) // number of tiles
            {
                // all frames same size
                _aFrame[0].w = (uint16_t)( _tw / -framesHorz );
                _aFrame[0].h = (uint16_t)( _th / -framesVert );
            }

            return bValid;
        }

        // Creates a dynamic texture that is meant to be updated & uploaded every frame
        // @see: MakeTexture() UploadTexture()
        // ========================================================================
        void
        MakeTexture( const uint16_t width, const uint16_t height, const uint8_t bpp = 32, uint8_t *pixels = 0 )
        {
            _tw = width;
            _th = height;

            _aFrame[0].x = 0;
            _aFrame[0].y = 0;
            _aFrame[0].w = _tw; // TileW is entire texture
            _aFrame[0].h = _th; // TileH is entire texture

            _nFrame = 1;

            // TODO: if this is currently a clone then this should fail...
            if( IsClone() )
                Game_Fatal( "Cloned sprite cannot be dynamically modified w/o modifying the parent as well" );

            if( pixels )
            {   // uncommon case
                _pData = pixels;
            } else {
                uint32_t nSizeTextureAtlas = width * height * (bpp / 8);
                _pData = malloc( nSizeTextureAtlas );
            }
        }

        // kick off the actual rendering to the GPU
        // ========================================================================
        void
        Render( bool bAltBlend = false )
        {
            if( IsHidden() )
                return;

            if( bAltBlend ) // blending over-ride
            {
// Ver 1
//                glBlendFunc( GL_ZERO, GL_SRC_ALPHA ); // AA == 0x00 black, 0xFF full lit
// Ver 2
                glBlendFunc( GL_SRC_ALPHA, GL_SRC_ALPHA ); // AA == 0x00 black, 0xFF full lit
            }

            const int nXY = 4; // # vertices xyz: 4
            const int nUV = 4; // # vertices uv : 4
            const int sXY = nXY * 3 * sizeof( float ); // total size in bytes
            const int sUV = nUV * 2 * sizeof( float ); // total size in bytes
            OpenglBufferData( _hvBuffer, _aXY, sXY, GL_STATIC_DRAW );
            OpenglBufferData( _htBuffer, _aUV, sUV, GL_STATIC_DRAW );

            AssertGL( "SPRITE: bad buffering" );
                Shader_Begin( SHADER_KTV );

                    Shader_Color4f( _rgba[0], _rgba[1], _rgba[2], _rgba[3] );
                    Shader_VertexPointer  ( _hvBuffer, 3, GL_FLOAT, 0, 0 );
                    Shader_TexCoordPointer( _htBuffer, 2, GL_FLOAT, 0, 0, false );
                    Shader_Texture( _hTexture, 0 );
                    glDrawArrays( GL_QUADS, 0, nXY );

//                if( bAltBlend ) // AABBGGRR tint support
//                {
// Ver 1
//                    glBlendFunc( GL_SRC_ALPHA, GL_ONE ); // GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
//                    glDrawArrays( GL_QUADS, 0, nXY );
//                }

                Shader_End();
            AssertGL( "SPRITE: bad shader" );

            if( bAltBlend ) // reset back to normal alpha blending
                glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        }

        // ========================================================================
        void render( SpriteBuffer_t *buffer )
        {
            glEnable( GL_BLEND );
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); // TODO: FIXME: Use pre-multiplied alpha

            const int nXY = buffer->nXY; // # vertices xyz
            const int nUV = buffer->nUV; // # vertices uv 
            const int sXY = nXY * 3 * sizeof( float ); // total size in bytes
            const int sUV = nUV * 2 * sizeof( float ); // total size in bytes
            OpenglBufferData( _hvBuffer, _aXY, sXY, GL_DYNAMIC_DRAW );
            OpenglBufferData( _htBuffer, _aUV, sUV, GL_DYNAMIC_DRAW );

            AssertGL( "SPRITE: bad buffering" );
                Shader_Begin( SHADER_KTV );
                    Shader_Color4f( _rgba[0], _rgba[1], _rgba[2], _rgba[3] );
                    Shader_VertexPointer  ( _hvBuffer, 3, GL_FLOAT, 0, 0 );
                    Shader_TexCoordPointer( _htBuffer, 2, GL_FLOAT, 0, 0, false );
                    Shader_Texture( _hTexture, 0 );
                    glDrawArrays( GL_QUADS, 0, nXY );
                Shader_End();
            AssertGL( "SPRITE: bad shader" );
        }

        // ========================================================================
        void
        SetAnimating( bool isAnimating )
        {
            if( isAnimating ) _bFlags |=  SPRITE_FLAG_IS_ANIMATING;
            else              _bFlags &= ~SPRITE_FLAG_IS_ANIMATING;
        }

        // ========================================================================
        void
        SetFlipReversed()
        {
            _bFlags |= SPRITE_FLAG_FLIP_IS_REVERSED;
        }

        // ========================================================================
        void
        unbindOpenGL()
        {
            if( _hvBuffer ) OpenglDeleteBuffer( _hvBuffer );
            if( _htBuffer ) OpenglDeleteBuffer( _htBuffer );
            if( _hkBuffer ) OpenglDeleteBuffer( _hkBuffer );
#if DEBUG
            _hvBuffer = 0;
            _htBuffer = 0;
            _hkBuffer = 0;
#endif
        }

        // ========================================================================
        void
        Unload()
        {
            // don't deallocate the child, which is just a reference, we deallocate the parent
            if (! IsClone())
                free( _pData );
        }

        /**
            @param pvCamera NOTE: Requires a camera, if not NULL will default to screen space
            @example   pSprite->Update( 0.f, &gvCameraLocation ); // World Space
                       pSprite->Update( 0.f, NULL              ); // Screen Space
        */
        // fill in all the render data
        void Update( const float deltaTime, Vector2f *pvCamera = NULL, uint32_t colorAABBGGRR = -1 )
        {
            // TODO: updatePhysics() ?
            if( _bFlags & SPRITE_FLAG_IS_ANIMATING )
            {
                _nAnimTimer += deltaTime;
                if( _nAnimTimer > _nAnimDuration )
                {
                    _nAnimTimer = 0.f;
                    _iFrame++;
                    if( _iFrame > _iAnimFrameEnd )
                        _iFrame = _iAnimFrameBegin;
                }
            }

            if( IsHidden() )
                return;

            _vLocationScreenSpace = WorldSpaceToScreenSpace( vLocation, pvCamera );
            _vLocationScreenSpace += _vDeltaImageSpace;

// TODO: Don't render if clipped !!!!
// Set Flag -- Clipped

            const SpriteRect *pDim = &_aFrame[ _iFrame ];
#if 1
            const float      fx = _vLocationScreenSpace.x;
            const float      fy = _vLocationScreenSpace.y;
            const float      fw = _scaleW * (float)pDim->w;
            const float      fh = _scaleH * (float)pDim->h;

            const float      tx = (float)pDim->x * _ootw;
            const float      ty = (float)pDim->y * _ooth;
            const float      tw = (float)pDim->w * _ootw;
            const float      th = (float)pDim->h * _ooth;
#else // Debug - draw entire texture atlas
            float fx = 0;
            float fy = 0;
            float fw = _tw;
            float fh = _th;
            float tx = 0.0;
            float ty = 0.0;
            float tw = 1.0;
            float th = 1.0;
#endif

#if ORTHO_BOT_LEFT
            _aXY[ 0] = fx   ; _aXY[ 1] = fy   ; _aXY[ 2] = 0.f;
            _aXY[ 3] = fx+fw; _aXY[ 4] = fy   ; _aXY[ 5] = 0.f;
            _aXY[ 6] = fx+fw; _aXY[ 7] = fy+fh; _aXY[ 8] = 0.f;
            _aXY[ 9] = fx   ; _aXY[10] = fy+fh; _aXY[11] = 0.f;
#else
            _aXY[ 0] = fx   ; _aXY[ 1] = fy+fh; _aXY[ 2] = 0.f;
            _aXY[ 3] = fx+fw; _aXY[ 4] = fy+fh; _aXY[ 5] = 0.f;
            _aXY[ 6] = fx+fw; _aXY[ 7] = fy   ; _aXY[ 8] = 0.f;
            _aXY[ 9] = fx   ; _aXY[10] = fy   ; _aXY[11] = 0.f;
#endif

            float tx0 = tx;
            float tx1 = tx+tw;

            if( _bFlags & SPRITE_FLAG_FLIP_X )
            {
                tx0 = tx+tw;
                tx1 = tx;
            }

            _aUV[0] = tx0; _aUV[1] =  ty+th;
            _aUV[2] = tx1; _aUV[3] =  ty+th;
            _aUV[4] = tx1; _aUV[5] =  ty   ;
            _aUV[6] = tx0; _aUV[7] =  ty   ;

            // TODO: This should be moved to SpriteBuffer_t
            //if( colorAABBGGRR )
            {
                uint8_t r = (colorAABBGGRR >>  0) & 0xFF;
                uint8_t g = (colorAABBGGRR >>  8) & 0xFF;
                uint8_t b = (colorAABBGGRR >> 16) & 0xFF;
                uint8_t a = (colorAABBGGRR >> 24) & 0xFF;

                Color4f_t color = ColorU8ToF32( r, g, b, a );
                _rgba[0] = color.r;
                _rgba[1] = color.g;
                _rgba[2] = color.b;
                _rgba[3] = color.a;
            }
        }

        /** For Dynamic Textures
        */
        // ========================================================================
        void
        UploadTexture( int format = GL_RGBA )
        {
            OpenglUpdateTexture( _hTexture, _tw, _th, _pData, format );
        }
};
