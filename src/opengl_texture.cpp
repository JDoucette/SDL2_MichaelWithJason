// TODO: Default Texture // 0 = 2x2 checkerboard // Move to Render_Startup() ? Update ShaderTexCoordPointer() if(tex==0) use default
const uint8_t gaDefaultTexture[] = {
    0xFF, 0x00, 0xFF, 0xFF, // pink
    0xFF, 0xFF, 0xFF, 0x7F, // white
    0xFF, 0x00, 0xFF, 0xFF, // pink
    0xFF, 0xFF, 0xFF, 0x7F, // white
};

// = Implementation =

// Format: GL_RGBA or GL_BGRA (not GL_RGB GL_RGBA8)
// ======================================================================== 
GLuint
OpenglCreateTexture( const uint16_t width, const uint16_t height, void * pixels, const int type )
{
    // TODO: We really should be pre-allocating a block of id's once and using those ... TextureIdManager_Startup() TextureManager_Shutdown()
    // http://timothylottes.blogspot.com/2014/01/modern-opengl-texture-usage.html
    GLuint hTexture;

    glEnable( GL_TEXTURE_2D ); // TODO: FIXME: This should only be done once in Game_Startup()!

    glGenTextures( 1, &hTexture );
    glBindTexture( GL_TEXTURE_2D, hTexture );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S    , GL_REPEAT ); // CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T    , GL_REPEAT ); // CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

// TIU - Texture Image Unit
//glActiveTexture( GL_TEXTURE0 + 0 );
//glTexEnvi( GL_TEXTURE_2D, GL_TEXTURE_ENV, GL_REPLACE );
//    glTexEnvi( GL_TEXTURE_2D, GL_TEXTURE_ENV, GL_MODULATE );
    int channels = (type == GL_BGRA) ? GL_RGBA8 : type;
    int format   = type;
    if( type == GL_RED )
    {
        // https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml
        // https://www.khronos.org/registry/gles/extensions/EXT/EXT_texture_rg.txt
        channels = GL_LUMINANCE; // Deprecated OpenGL 4.x GL_LUMINANCE -> GL_RED
        format   = GL_RED;
    }

    // target, level, internalFormat, width, height, border, format, type, texels
    // http://stackoverflow.com/questions/9848901/opengl-use-single-channel-texture-as-alpha-channel-to-display-text
    glTexImage2D( GL_TEXTURE_2D
        , 0        // mipmap level 0
        , channels // internal format: RGBA8 = # of channels
        , width  
        , height 
        , 0        // no border
        , format   //  transfer format = GL_RGBA or GL_BGRA (Preferred on Windows)
        , GL_UNSIGNED_BYTE // type
        , pixels
    );

    return hTexture;
}

void
OpenglUpdateTexture( const GLuint hTexture, const int width, const int height, void * pixels, const int type )
{
    glBindTexture( GL_TEXTURE_2D, hTexture );

    int channels = (type == GL_BGRA) ? GL_RGBA8 : type;
    int format   = type;
    if( type == GL_RED )
    {
        // https://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml
        channels = GL_LUMINANCE; // Deprecated OpenGL 4.x GL_LUMINANCE -> GL_RED
        format   = GL_RED;
    }

    // TODO: Use glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, pixels );

    // target, level, internalFormat, width, height, border, format, type, texels
    // http://stackoverflow.com/questions/9848901/opengl-use-single-channel-texture-as-alpha-channel-to-display-text
    glTexImage2D( GL_TEXTURE_2D
        , 0        // mipmap level 0
        , channels // internal format: RGBA8
        , width  
        , height 
        , 0        // no border
        , format   //  transfer format = GL_RGBA or GL_BGRA (Preferred on Windows)
        , GL_UNSIGNED_BYTE // type
        , pixels
    );
}
