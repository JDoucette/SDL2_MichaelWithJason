// = Types =
    enum Shaders_e
    {
         SHADER_CKV   // Variable Color modulated with Constant Kolor, Variable Position
        ,SHADER_CKTV  // Variable Color modulated with Constant Kolor and Texture, Variable Position
        ,SHADER_CV    // Variable Color, Variable Position
        ,SHADER_KV    // Constant Kolor, Variable Position
        ,SHADER_KTV   // Constant Kolor modulated with Texture, Variable Position
        ,SHADER_TV    // Variable Position, Texture
        ,SHADER_KTVu16x16  // Constant Kolor modulated with Texture/16, Variable Position
        ,SHADER_TVu16x16   // Variable Position, Texture/16
        ,SHADER_FONT_16x16 // Constant Color modulated with Red Texture, KTV, with uv coords 16x16 on texture
        ,SHADER_JET
        ,SHADER_DEPTH
        ,NUM_SHADERS
    };

// = Macros =
#if DEBUG
    #define AssertGL(x) AssertOpenGL(__FILE__,__LINE__,x)
#else
    #define AssertGL(x) ;
#endif

// Globals (Public)

    void AssertOpenGL( const char *pFileName, size_t nLine, const char * pDebugMessage = 0 );

    void   Shader_Begin   ( const int iShader );
    void   Shader_Color1v ( const float aRGBA[ 4 ] );
    void   Shader_Color4f ( const float r, const float g, const float b, const float a );
    void   Shader_ColorPointer   ( GLuint buffer, const int size, const int type, const int stride, const int offset );
    void   Shader_End     ();
    void   Shader_Shutdown();
    bool   Shader_Startup ( bool verbose = false );
    void   Shader_TexCoordPointer( GLuint buffer, const int size, const int type, const int stride, const int offset, const bool bNormalize = false );
    void   Shader_Texture( GLuint hTexture, const int iTexture = 0 );
    void   Shader_VertexPointer  ( GLuint buffer, const int size, const int type, const int stride, const int offset );

    GLuint OpenglCreateBuffer();
    void   OpenglDeleteBuffer( GLuint buffer );
    GLuint OpenglBufferData( const GLuint buffer, const void * data, const int sizeTotalBytes, const int usage );

