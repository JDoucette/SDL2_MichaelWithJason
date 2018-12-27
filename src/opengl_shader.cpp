// = Includes =
//    #include <stdio.h>

// Public

    // Note: Must be kept in sync:  opengl_funcs.h opengl_shaders.cpp
    // Opengl 2.x

// Shaders - Attributes
        PFNGLGETATTRIBLOCATIONPROC        oglGetAttribLocation           = 0;

// Shaders - Buffers Create/Destroy
        PFNGLBINDBUFFERARBPROC            oglBindBuffer                  = 0;
        PFNGLBUFFERDATAARBPROC            oglBufferDataARB               = 0;
        PFNGLBUFFERSUBDATAARBPROC         oglBufferSubDataARB            = 0;
        PFNGLDELETEBUFFERSPROC            oglDeleteBuffers               = 0;
        PFNGLGENBUFFERSPROC               oglGenBuffers                  = 0;

// Shaders - Buffers
//      PFNGLDISABLEVERTEXATTRIBARRAYPROC oglDisableVertexAttribArrayARB = 0;
        PFNGLENABLEVERTEXATTRIBARRAYPROC  oglEnableVertexAttribArray     = 0;
        PFNGLVERTEXATTRIBPOINTERPROC      oglVertexAttribPointer         = 0;

// Shaders - Compile/Link
        PFNGLATTACHSHADERPROC             oglAttachShader                = 0;
        PFNGLCOMPILESHADERPROC            oglCompileShader               = 0;
        PFNGLLINKPROGRAMPROC              oglLinkProgram                 = 0;
        PFNGLSHADERSOURCEPROC             oglShaderSource                = 0;
 
// Shaders - Create/Destroy
        PFNGLCREATEPROGRAMPROC            oglCreateProgram               = 0;
        PFNGLCREATESHADERPROC             oglCreateShader                = 0;
        PFNGLDELETEPROGRAMPROC            oglDeleteProgram               = 0;
        PFNGLDELETESHADERPROC             oglDeleteShader                = 0;
        PFNGLUSEPROGRAMPROC               oglUseProgram                  = 0;

// Shaders - Debug
//      PFNGLGETINFOLOGARBPROC            oglGetInfoLogARB               = 0;
//      PFNGLGETOBJECTPARAMETERIVARBPROC  oglGetObjectParameterivARB     = 0;
        PFNGLGETPROGRAMINFOLOGPROC        oglGetProgramInfoLog           = 0;
        PFNGLGETSHADERIVPROC              oglGetShaderiv                 = 0;
//      PFNGLGETSHADERINFOLOGPROC         oglGetShaderInfoLog            = 0;

// Shaders - Uniforms
        PFNGLGETUNIFORMLOCATIONPROC       oglGetUniformLocation          = 0;
        PFNGLUNIFORM1IPROC                oglUniform1i                   = 0;
        PFNGLUNIFORM4FPROC                oglUniform4f                   = 0;
        PFNGLUNIFORMMATRIX4FVPROC         oglUniformMatrix4fv            = 0;

// Shaders - Texture
        PFNGLACTIVETEXTUREPROC            oglActiveTexture               = 0; // WIN32

// glBindTexture
        PFNGLDRAWARRAYSEXTPROC            oglDrawArrays                  = 0; // ATI // WIN32

// = Private =

    const char *gaVertSources[ NUM_SHADERS ] =
    {
// VERT SHADER_CKV // no alpha
        "\
        uniform   mat4 umProj    ; \
        uniform   mat4 umView    ; \
        varying   vec4 vvColor   ; \
        attribute vec3 avColor   ; \
        attribute vec3 avPosition; \
                                   \
        void main()                \
        {                          \
            gl_Position = umProj * umView * vec4( avPosition, 1.0 ); \
            vvColor     = vec4( avColor, 1.0 );                      \
        }\n"

// VERT SHADER_CKTV
        ,"\
        uniform   mat4 umProj    ; \
        uniform   mat4 umView    ; \
        attribute vec4 avColor   ; \
        varying   vec4 vvColor   ; \
        attribute vec2 avTexCoord; \
        varying   vec2 vvTexCoord; \
        attribute vec3 avPosition; \
                                   \
        void main()                \
        {                          \
            gl_Position = umProj * umView * vec4( avPosition, 1.0 ); \
            vvColor     = avColor;                                   \
            vvTexCoord  = avTexCoord;                                \
        }\n"

// VERT SHADER_CV
        ,"\
        uniform   mat4 umProj ;                        \
        uniform   mat4 umView ;                        \
        attribute vec4 avColor;                        \
        varying   vec4 vvColor;                        \
                                                       \
        void main()                                    \
        {                                              \
            gl_Position = umProj * umView * gl_Vertex; \
            vvColor     = avColor;                     \
        }\n"

// VERT SHADER_KV
//            vvColor     = gl_Color;                    
//            gl_Position = umProj * umView * gl_Vertex; 

        ,"\
        uniform   mat4 umProj    ; \
        uniform   mat4 umView    ; \
        attribute vec3 avPosition; \
                                   \
        void main()                \
        {                          \
            gl_Position = umProj * umView * vec4( avPosition, 1.0 ); \
        }\n"

// VERT SHADER KTV
        ,"\
        uniform   mat4 umProj    ; \
        uniform   mat4 umView    ; \
        attribute vec2 avTexCoord; \
        varying   vec2 vvTexCoord; \
        attribute vec3 avPosition; \
                                   \
        void main()                \
        {                          \
            vec4 p = vec4( avPosition.x, avPosition.y, 0.0, 1.0 ); \
            gl_Position = umProj * umView * p; \
            vvTexCoord  = avTexCoord;          \
        }\n"

// VERT SHADER TV

        ,"\
        uniform   mat4 umProj    ; \
        uniform   mat4 umView    ; \
        attribute vec2 avTexCoord; \
        varying   vec2 vvTexCoord; \
        attribute vec3 avPosition; \
                                   \
        void main()                \
        {                          \
            gl_Position = umProj * umView * vec4( avPosition, 1.0 ); \
            vvTexCoord  = avTexCoord;                                \
        }\n"

// VERT SHADER KTVu16x16
        ,"\
        uniform   mat4 umProj    ; \
        uniform   mat4 umView    ; \
        attribute vec2 avTexCoord; \
        varying   vec2 vvTexCoord; \
        attribute vec3 avPosition; \
                                   \
        void main()                \
        {                          \
            gl_Position = umProj * umView * vec4( avPosition, 1.0 ); \
            vvTexCoord = avTexCoord / 16.0;                          \
        }\n"

// VERT SHADER TVu16x16
        ,"\
        uniform   mat4 umProj    ; \
        uniform   mat4 umView    ; \
        attribute vec2 avTexCoord; \
        varying   vec2 vvTexCoord; \
        attribute vec3 avPosition; \
                                   \
        void main()                \
        {                          \
            gl_Position = umProj * umView * vec4( avPosition, 1.0 ); \
            vvTexCoord = avTexCoord / 16.0;                          \
        }\n"

// VERT SHADER_FONT_16x16
        ,"\
        uniform   mat4 umProj    ; \
        uniform   mat4 umView    ; \
        attribute vec2 avTexCoord; \
        varying   vec2 vvTexCoord; \
        attribute vec2 avPosition; \
                                   \
        void main()                \
        {                          \
            gl_Position = umProj * umView * vec4( avPosition, 0.0, 1.0 ); \
            vvTexCoord  = avTexCoord / 16.0;                              \
        }\n"

// VERT SHADER_JET
        ,"\
        uniform mat4 umProj ;                                     \
        uniform mat4 umView ;                                     \
        uniform vec4 uvColor;                                     \
        varying vec4 vvColor;                                     \
        void main()                                               \
        {                                                         \
            gl_Position = umProj * umView * gl_Vertex;            \
            float k = 4.0 * gl_Vertex.y / 255.0;                  \
            float r = clamp( min( k - 1.5, 4.5 - k), 0.0, 1.0 );  \
            float g = clamp( min( k - 0.5, 3.5 - k), 0.0, 1.0 );  \
            float b = clamp( min( k + 0.5, 2.5 - k), 0.0, 1.0 );  \
            vvColor = vec4( r, g, b, 1.0 );                       \
            vvColor *= uvColor;                                   \
        }\n"

//            vvColor *= (gl_Vertex.y / 512.0) 

// SHADER_DEPTH
        ,"\
        uniform mat4 umProj;                           \
        uniform mat4 umView;                           \
                                                       \
        void main()                                    \
        {                                              \
            gl_Position = umProj * umView * gl_Vertex; \
        }\n"
    };

// ------------------------------------------------------------------------ 
    const char *gaFragSources[ NUM_SHADERS ] =
    {
// FRAG SHADER_CKV
        "\
        uniform vec4      uvColor   ; \
        varying vec4      vvColor   ; \
                                      \
        void main()                   \
        {                             \
            gl_FragColor = uvColor * vvColor;\
        }\n"

// FRAG SHADER_CKTV
        ,"\
        uniform vec4      uvColor   ; \
        varying vec4      vvColor   ; \
        uniform sampler2D utDiffuse ; \
        varying vec2      vvTexCoord; \
                                      \
        void main()                   \
        {                             \
            vec4 texel   = texture2D( utDiffuse, vvTexCoord );\
            gl_FragColor = uvColor * vvColor * texel;         \
        }\n"

// FRAG SHADER_CV
        ,"\
        varying vec4      vvColor   ; \
                                      \
        void main()                   \
        {                             \
            gl_FragColor = vvColor;   \
        }\n"

// FRAG SHADER_KV
        ,"\
        uniform vec4      uvColor   ; \
                                      \
        void main()                   \
        {                             \
            gl_FragColor = uvColor;   \
        }\n"

// FRAG SHADER_KTV
        ,"\
        uniform vec4      uvColor   ; \
        uniform sampler2D utDiffuse ; \
        varying vec2      vvTexCoord; \
                                      \
        void main()                   \
        {                             \
            vec4 texel   = texture2D( utDiffuse, vvTexCoord );\
            gl_FragColor = texel * uvColor;                   \
        }\n"

// FRAG SHADER_TV
//            gl_FragColor = vec4(0.5,0.75,1.0,1.0) * texel.a; 
        ,"\
        uniform sampler2D utDiffuse ; \
        varying vec2      vvTexCoord; \
                                      \
        void main()                   \
        {                             \
            vec4 texel   = texture2D( utDiffuse, vvTexCoord );\
            gl_FragColor = texel;                             \
        }\n"

// FRAG SHADER_KTVu16x16
        ,"\
        uniform vec4      uvColor   ; \
        uniform sampler2D utDiffuse ; \
        varying vec2      vvTexCoord; \
                                      \
        void main()                   \
        {                             \
            vec4 texel   = texture2D( utDiffuse, vvTexCoord );\
            gl_FragColor = texel * uvColor;                   \
        }\n"

// FRAG SHADER_TVu16x16
        ,"\
        uniform sampler2D utDiffuse ; \
        varying vec2      vvTexCoord; \
                                      \
        void main()                   \
        {                             \
            vec4 texel   = texture2D( utDiffuse, vvTexCoord );\
            gl_FragColor = texel;                             \
        }\n"

// FRAG SHADER_FONT_16x16
        ,"\
        uniform vec4      uvColor   ; \
        uniform sampler2D utDiffuse ; \
        varying vec2      vvTexCoord; \
                                      \
        void main()                   \
        {                             \
            vec4  texel  = texture2D( utDiffuse, vvTexCoord );\
            float font   = texel.r;                           \
            gl_FragColor = uvColor * font;                    \
        }\n"

// FRAG SHADER_JET
        ,"\
        uniform vec3 uvNormal;     \
        varying vec4 vvColor;      \
                                   \
        void main()                \
        {                          \
            gl_FragColor = vvColor;\
        }\n"

// FRAG SHADER_DEPTH
        ,"\
        void main()                                                                        \
        {                                                                                  \
            float delta = gl_DepthRange.far - gl_DepthRange.near;                          \
            float depth = (2.0 * gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) / delta; \
            float clip  = depth / gl_FragCoord.w;                                          \
            gl_FragColor = vec4( (clip * 0.5 ) + 0.5 );                                    \
        }\n"

    };

    const char *gapDesc[ NUM_SHADERS ] =
    {
         "CKV: Var Color + Const Kolor + Var Vertex"
        ,"CKTV: Var Color * Const Kolor * Texture, Var Vertex"
        ,"CV: Var Color, Var Vertex"
        ,"KV: Const Color, Var Vertex"
        ,"KTV: Const Kolor * Texture, Var Vertex"
        ,"TV: Var Vertex + TexCoord"
        ,"KTV16: Const Kolor * Texture/16, Var Vertex"
        ,"TV16: Texture/16, Var Vertex"
        ,"Font16x16: Texture/16, Var Vertex"
        ,"Jet Color"
        ,"Depth Color"
    };

    struct Shader_t
    {
        // On OSX GLhandleARB is void*
        GLuint hProg;
        GLuint hVert;
        GLuint hFrag;

        // Uniforms - shader consts
        GLint humProj      ; // mat4
        GLint humView      ; // mat4
        GLint huvColor     ; // vec4
        GLint hutTexture[4]; // tex2 // albedo, specular, etc

        // Attributes - vertex streams
        GLint havPosition;
        GLint havColor   ;
        GLint havTexCoord;

        // FX
        GLint hunTime;
        GLint hunDuration;
    };

    int         giShader     = 0;
    Shader_t    gaShaders[ NUM_SHADERS ];
    Shader_t   *gpShader     = 0; // current shader
    Shader_t   *gpShaderPrev = 0;

#if DEBUG
    bool gbShadersDebug = false;
    #define SHADER_VERBOSE if( gbShadersDebug )    
#else
    #define SHADER_VERBOSE if(0)
#endif

// Private
    GLuint shaderCompile ( const int iShader, const char * shaderSource, const int shaderType );
    bool   shaderCreate  ( const int iShader );
    bool   shaderDestroy ( const int iShader );

// Implementation

// = Private =

// ------------------------------------------------------------------------ 
GLuint shaderCompile( const int iShader, const char * shaderSource, const int shaderType )
{
    (void) iShader;

    GLint  status;
    GLuint shader;

AssertGL( "> ShaderCompile"  ); shader = oglCreateShader( shaderType );
AssertGL( "  create shader"  ); oglShaderSource( shader, 1, &shaderSource, NULL );
AssertGL( "  shader source"  ); oglCompileShader( shader );
//AssertGL( "  compile shader" ); oglGetObjectParameterivARB( shader, GL_OBJECT_COMPILE_STATUS_ARB, &status );
AssertGL( "  compile shader" ); oglGetShaderiv( shader, GL_OBJECT_COMPILE_STATUS_ARB, &status );
AssertGL( "  shader status"  );

//VERBOSE( V_OPENGL )
SHADER_VERBOSE
    {
        TRACE( "  Shader %s : %s\n",
            shaderType == GL_VERTEX_SHADER_ARB
            ? "Vert"
            : shaderType == GL_FRAGMENT_SHADER_ARB
                ? "Frag"
                : "???"
            , status
            ? "pass"
            : "FAIL"
        );
    }

    if( status == 0 )
    {
#if DEBUG || PROFILE
        const char * pType = (shaderType == GL_VERTEX_SHADER_ARB) ? "Vert" : "Frag";
#endif
        TRACE( "ERROR: Shader #%d [%s] (%s) failed to compile!\n", iShader, pType, gapDesc[ iShader ] );
        shader = (GLuint)NULL;
    }

    return shader;
}

// ------------------------------------------------------------------------ 
bool shaderCreate( const int iShader )
{
    gpShader = &gaShaders[ iShader ];
    const char *pVertSource = gaVertSources[ iShader ];
    const char *pFragSource = gaFragSources[ iShader ];

AssertGL( "> ShadersCreate" );  gpShader->hProg = oglCreateProgram();
AssertGL( "  prog created"  );  gpShader->hVert = shaderCompile( iShader, pVertSource, GL_VERTEX_SHADER_ARB   );
AssertGL( "  vert compiled" );  gpShader->hFrag = shaderCompile( iShader, pFragSource, GL_FRAGMENT_SHADER_ARB );
AssertGL( "  frag compiled" );
    const GLuint program = gpShader->hProg;

/*VERBOSE( V_OPENGL )*/ 
SHADER_VERBOSE {                { TRACE( "#%d - %s\n", iShader, gapDesc[ iShader ] );                         } }
SHADER_VERBOSE {  if( program ) { TRACE( "     [%d] Prog: %08X\n", iShader, (unsigned int) gpShader->hProg ); } }
SHADER_VERBOSE {  if( program ) { TRACE( "     [%d] Vert: %08X\n", iShader, (unsigned int) gpShader->hVert ); } }
SHADER_VERBOSE {  if( program ) { TRACE( "     [%d] Frag: %08X\n", iShader, (unsigned int) gpShader->hFrag ); } }

AssertGL( "  shaders created"   ); oglAttachShader( program, gpShader->hVert );
AssertGL( "  attached vertex"   ); oglAttachShader( program, gpShader->hFrag );
AssertGL( "  attached fragment" ); oglLinkProgram ( program                  );
AssertGL( "  program linked"    );

    const int     LOG_SIZE = 1023;
    /* */ char    aLog[ LOG_SIZE + 1 ];
    /* */ GLsizei nLog = 0;
    if( oglGetProgramInfoLog   ) oglGetProgramInfoLog  ( program, LOG_SIZE, &nLog, aLog );
//    else
//    if( oglGetInfoLogARB       ) oglGetInfoLogARB      ( program, LOG_SIZE, &nLog, aLog );
//    else
//    if( oglGetShaderInfoLogARB ) oglGetShaderInfoLogARB( program, LOG_SIZE, &nLog, aLog );

    aLog[ LOG_SIZE ] = 0;

//SHADER_VERBOSE
    if( nLog > 0 )
    {
        TRACE( "[%d]: %s\n", iShader, aLog );
    }

    oglUseProgram( program );

    switch( iShader )
    {
        case SHADER_CKV:
            gpShader->humProj     = oglGetUniformLocation( program, "umProj"     ); // vert
            gpShader->humView     = oglGetUniformLocation( program, "umView"     ); // vert
            gpShader->huvColor    = oglGetUniformLocation( program, "uvColor"    ); // frag
            gpShader->havColor    = oglGetAttribLocation ( program, "avColor"    );
            gpShader->havPosition = oglGetAttribLocation ( program, "avPosition" );
            break;

        case SHADER_CKTV:
            gpShader->humProj       = oglGetUniformLocation( program, "umProj"     ); // vert
            gpShader->humView       = oglGetUniformLocation( program, "umView"     ); // vert
            gpShader->huvColor      = oglGetUniformLocation( program, "uvColor"    ); // frag
            gpShader->havColor      = oglGetAttribLocation ( program, "avColor"    );
            gpShader->havPosition   = oglGetAttribLocation ( program, "avPosition" );
            gpShader->havTexCoord   = oglGetAttribLocation ( program, "avTexCoord" ); // vert
            gpShader->hutTexture[0] = oglGetUniformLocation( program, "utDiffuse"  ); // vert+frag
            break;

        case SHADER_CV:
            gpShader->humProj  = oglGetUniformLocation( program, "umProj"  ); // vert
            gpShader->humView  = oglGetUniformLocation( program, "umView"  ); // vert
            break;

        case SHADER_KV:
            gpShader->humProj     = oglGetUniformLocation( program, "umProj"  ); // vert
            gpShader->humView     = oglGetUniformLocation( program, "umView"  ); // vert
            gpShader->huvColor    = oglGetUniformLocation( program, "uvColor" ); // frag
            gpShader->havPosition = oglGetAttribLocation ( program, "avPosition" ); // vert
            break;

        case SHADER_KTV:
        case SHADER_KTVu16x16:
            gpShader->humProj       = oglGetUniformLocation( program, "umProj"  ); // vert
            gpShader->humView       = oglGetUniformLocation( program, "umView"  ); // vert
            gpShader->huvColor      = oglGetUniformLocation( program, "uvColor" ); // frag
            gpShader->havPosition   = oglGetAttribLocation ( program, "avPosition" ); // vert
            gpShader->havTexCoord   = oglGetAttribLocation ( program, "avTexCoord" ); // vert
            gpShader->hutTexture[0] = oglGetUniformLocation( program, "utDiffuse"  ); // vert+frag
            break;

        case SHADER_TV:
        case SHADER_TVu16x16:
            gpShader->humProj       = oglGetUniformLocation( program, "umProj"    ); // vert
            gpShader->humView       = oglGetUniformLocation( program, "umView"    ); // vert
            gpShader->havPosition   = oglGetAttribLocation ( program, "avPosition" ); // vert
            gpShader->havTexCoord   = oglGetAttribLocation ( program, "avTexCoord" ); // vert
            gpShader->hutTexture[0] = oglGetUniformLocation( program, "utDiffuse"  ); // vert+frag
            break;

        case SHADER_FONT_16x16:
            gpShader->humProj       = oglGetUniformLocation( program, "umProj"  ); // vert
            gpShader->humView       = oglGetUniformLocation( program, "umView"  ); // vert
            gpShader->huvColor      = oglGetUniformLocation( program, "uvColor" ); // frag
            gpShader->havPosition   = oglGetAttribLocation ( program, "avPosition" ); // vert
            gpShader->havTexCoord   = oglGetAttribLocation ( program, "avTexCoord" ); // vert
            gpShader->hutTexture[0] = oglGetUniformLocation( program, "utDiffuse"  ); // vert+frag

        case SHADER_JET:
            gpShader->humProj  = oglGetUniformLocation( program, "umProj"  ); // vert
            gpShader->humView  = oglGetUniformLocation( program, "umView"  ); // vert
            gpShader->huvColor = oglGetUniformLocation( program, "uvColor" ); // frag
            break;

        case SHADER_DEPTH:
            gpShader->humProj = oglGetUniformLocation( program, "umProj" ); // vert
            gpShader->humView = oglGetUniformLocation( program, "umView" ); // vert
            break;

        default:
            break;
    }

    oglUseProgram( 0 );
    gpShader = 0;

AssertGL( "< ShadersCreate" );

    return true;
}

// ------------------------------------------------------------------------ 
bool shaderDestroy( const int iShader )
{
    Shader_t *pShader = &gaShaders[ iShader ];

    oglDeleteShader  ( pShader->hFrag );
    oglDeleteShader  ( pShader->hVert );
    oglDeleteProgram( pShader->hProg );

    pShader->hProg = 0;
    pShader->hVert = 0;
    pShader->hFrag = 0;

    return true;
}



// = Public =

// ======================================================================== 
void AssertOpenGL( const char *pFileName, size_t nLine, const char * pDebugMessage /* = 0 */ )
{
    (void) pFileName;
    (void) nLine;
    (void) pDebugMessage;

    int iError = glGetError();
    if( iError == GL_NO_ERROR )
        return;

#if DEBUG || PROFILE
    const char * pText = pDebugMessage ? pDebugMessage : "";
    TRACE( "ERROR: File: %s, Line: %d, OPENGL: %d @ %s\n", pFileName, (unsigned int) nLine, iError, pText );
#endif

    // GameShutdown();
    exit( 1 );
}

// ======================================================================== 
void Shader_Begin( const int iShader )
{
    giShader = iShader;
    gpShader = &gaShaders[ iShader ];
    oglUseProgram( gpShader->hProg );

    const float *pProj = matrixPeekProj();
    const float *pView = matrixPeekView();

    // glUniformMatrix4fv( GLint location, GLsizei count, GLboolean transpose, const GLfloat *value )
    oglUniformMatrix4fv( gpShader->humProj, 1, 0, pProj );
    oglUniformMatrix4fv( gpShader->humView, 1, 0, pView );
}

// ======================================================================== 
void Shader_Color1v( const float aRGBA[ 4 ] )
{
    oglUniform4f( gpShader->huvColor, aRGBA[0], aRGBA[1], aRGBA[2], aRGBA[3] ); 
}

// ======================================================================== 
void Shader_Color4f( const float r, const float g, const float b, const float a )
{
// TODO: DEBUG: if( !gpShader->huvColor ) VERBOSE( V_SHADER_VARIABLES ) TRACE(  "WARN: OPENGL: Shader #%d doesn't have 'uvColor'\n", giShader );
    oglUniform4f( gpShader->huvColor, r, g, b, a );
}

// ======================================================================== 
void Shader_ColorPointer( GLuint buffer, const int elementSize, const int type, const int stride, const int offset )
{
    oglBindBuffer( GL_ARRAY_BUFFER, buffer );
// TODO: DEBUG: if( !gpShader->huvColor ) VERBOSE( V_SHADER_VARIABLES ) TRACE(  "WARN: OPENGL: Shader #%d doesn't have 'avColor'\n", giShader );
    oglVertexAttribPointer( gpShader->havColor, elementSize, type, false, stride, (GLvoid*) (intptr_t) offset );
    oglEnableVertexAttribArray( gpShader->havColor );
}

// ======================================================================== 
void Shader_End()
{
    giShader = 0;
    gpShader = NULL;
}

// ======================================================================== 
void Shader_Shutdown()
{
    oglUseProgram( 0 );
    gpShader = NULL;

    for( int iShader = 0; iShader < NUM_SHADERS; iShader++ )
    {
        if( !shaderDestroy( iShader ) )
        {
            TRACE( "WARN: OPENGL: Unable to delete shader #%d\n", iShader );
        }
    }
}

// ======================================================================== 
bool Shader_Startup( bool verbose )
{
    (void) verbose;

#if DEBUG
    gbShadersDebug = verbose;
#endif

AssertGL( "> ShadersStartup()" );

    bool haveShaders = 
        SDL_GL_ExtensionSupported( "GL_ARB_shader_objects"       )
    &&  SDL_GL_ExtensionSupported( "GL_ARB_shading_language_100" )
    &&  SDL_GL_ExtensionSupported( "GL_ARB_vertex_shader"        )
    &&  SDL_GL_ExtensionSupported( "GL_ARB_fragment_shader"      );

    if( !haveShaders )
    {
        TRACE( "ERROR: Unable to find OpenGL shader support!\n" );
        return false;
    }

//    glDeleteObjectARB            = (PFNGLDELETEOBJECTARBPROC        ) SDL_GL_GetProcAddress( "glDeleteObjectARB"         );

// Shaders - Attributes
    oglGetAttribLocation          = (PFNGLGETATTRIBLOCATIONPROC        ) SDL_GL_GetProcAddress( "glGetAttribLocation"       );

// Shaders - Buffers Create/Destroy
    oglBindBuffer                  = (PFNGLBINDBUFFERPROC              ) SDL_GL_GetProcAddress( "glBindBuffer"              );
    oglBufferDataARB               = (PFNGLBUFFERDATAARBPROC           ) SDL_GL_GetProcAddress( "glBufferData"              );
    oglBufferSubDataARB            = (PFNGLBUFFERSUBDATAARBPROC        ) SDL_GL_GetProcAddress( "glBufferSubData"           );
    oglDeleteBuffers               = (PFNGLDELETEBUFFERSPROC           ) SDL_GL_GetProcAddress( "glDeleteBuffers"           );
    oglGenBuffers                  = (PFNGLGENBUFFERSARBPROC           ) SDL_GL_GetProcAddress( "glGenBuffers"              );

// Shaders - Buffers
//  oglDisableVertexAttribArrayARB = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) SDL_GL_GetProcAddress( "glDisableVertexAttribArray");
    oglEnableVertexAttribArray     = (PFNGLENABLEVERTEXATTRIBARRAYPROC ) SDL_GL_GetProcAddress( "glEnableVertexAttribArray" );
    oglVertexAttribPointer         = (PFNGLVERTEXATTRIBPOINTERPROC     ) SDL_GL_GetProcAddress( "glVertexAttribPointer"     );

// Shaders - Compile/Link
// requires ARB??
    oglAttachShader                = (PFNGLATTACHSHADERPROC            ) SDL_GL_GetProcAddress( "glAttachShader"            );
    oglCompileShader               = (PFNGLCOMPILESHADERPROC           ) SDL_GL_GetProcAddress( "glCompileShader"           );
    oglLinkProgram                 = (PFNGLLINKPROGRAMPROC             ) SDL_GL_GetProcAddress( "glLinkProgram"             );
    oglShaderSource                = (PFNGLSHADERSOURCEPROC            ) SDL_GL_GetProcAddress( "glShaderSource"            );

// Shaders - Create/Destroy
    // ARB : glCreateShaderObjectARB
    // SPEC: glCreateShader
    oglCreateProgram               = (PFNGLCREATEPROGRAMPROC           ) SDL_GL_GetProcAddress( "glCreateProgram"           );
    oglCreateShader                = (PFNGLCREATESHADERPROC            ) SDL_GL_GetProcAddress( "glCreateShader"            );
    oglDeleteProgram               = (PFNGLDELETEPROGRAMPROC           ) SDL_GL_GetProcAddress( "glDeleteProgram"           );
    oglDeleteShader                = (PFNGLDELETESHADERPROC            ) SDL_GL_GetProcAddress( "glDeleteShader"            );
    oglUseProgram                  = (PFNGLUSEPROGRAMPROC              ) SDL_GL_GetProcAddress( "glUseProgram"              );

// Shaders - Debug
//  oglGetInfoLogARB               = (PFNGLGETINFOLOGARBPROC           ) SDL_GL_GetProcAddress( "glGetInfoLogARB"           ); // GL_ARB_shader_objects
//  oglGetObjectParameterivARB     = (PFNGLGETOBJECTPARAMETERIVARBPROC ) SDL_GL_GetProcAddress( "glGetObjectParameterivARB" );
  oglGetProgramInfoLog             = (PFNGLGETPROGRAMINFOLOGPROC       ) SDL_GL_GetProcAddress( "glGetProgramInfoLog"       );
//  oglGetShaderInfoLog            = (PFNGLGETSHADERINFOLOGPROC        ) SDL_GL_GetProcAddress( "glGetShaderInfoLog"        );
    oglGetShaderiv                 = (PFNGLGETSHADERIVPROC             ) SDL_GL_GetProcAddress( "glGetShaderiv"             );

// Shaders - Uniforms
    oglGetUniformLocation          = (PFNGLGETUNIFORMLOCATIONPROC      ) SDL_GL_GetProcAddress( "glGetUniformLocation"      );
    oglUniform1i                   = (PFNGLUNIFORM1IPROC               ) SDL_GL_GetProcAddress( "glUniform1i"               );
    oglUniform4f                   = (PFNGLUNIFORM4FPROC               ) SDL_GL_GetProcAddress( "glUniform4f"               );
    oglUniformMatrix4fv            = (PFNGLUNIFORMMATRIX4FVPROC        ) SDL_GL_GetProcAddress( "glUniformMatrix4fv"        );

// Shaders - Texture
//    glGenTexturesARB   = () SDL_GL_GetProcAddress( "glGenTextures"   );
//    glBindTextureARB   = () SDL_GL_GetProcAddress( "glBindTexture"   );
//    glTexParameteriARB = () SDL_GL_GetProcAddress( "glTexParameteri" );
//    glTexImage2DARB    = () SDL_GL_GetProcAddress( "glTexImage2D"    );
    oglActiveTexture              = (PFNGLACTIVETEXTUREPROC            ) SDL_GL_GetProcAddress( "glActiveTexture"           ); // WIN32
    oglDrawArrays                 = (PFNGLDRAWARRAYSEXTPROC            ) SDL_GL_GetProcAddress( "glDrawArrays"              ); // ATI // WIN32

AssertGL( "  found GL functions" );

//SHADER_VERBOSE { TRACE( "oglGetInfoLogARB......: %p\n", oglGetInfoLogARB       ); }
//SHADER_VERBOSE { TRACE( "oglGetShaderInfoLog   : %p\n", oglGetShaderInfoLog    ); }
//SHADER_VERBOSE { TRACE( "oglGetShaderInfoLogARB: %p\n", oglGetShaderInfoLogARB ); }
  SHADER_VERBOSE { TRACE( "oglGetProgramInfoLog  : %p\n", oglGetProgramInfoLog   ); }

    for( int iShader = 0; iShader < NUM_SHADERS; iShader++ )
    {
        if( !shaderCreate( iShader ) )
        {
            TRACE( "ERROR: Failed to create shader #%d\n", iShader );
            return false;
        }
    }

AssertGL( "< ShaderStartup()" );

    return true;
}

// type: GL_FLOAT
// ======================================================================== 
void Shader_TexCoordPointer( GLuint buffer, const int elementSize, const int type, const int stride, const int offset, const bool bNormalize /* = false */ )
{
    oglBindBuffer( GL_ARRAY_BUFFER, buffer );
    oglVertexAttribPointer(     gpShader->havTexCoord, elementSize, type, bNormalize, stride, (GLvoid*) (intptr_t) offset );
    oglEnableVertexAttribArray( gpShader->havTexCoord );
}

// ======================================================================== 
void Shader_Texture( GLuint hTexture, const int iTexture /* = 0 */ )
{
    oglActiveTexture( GL_TEXTURE0 + iTexture ); // WIN32
    glBindTexture( GL_TEXTURE_2D, hTexture );
    oglUniform1i( gpShader->hutTexture[ iTexture ], iTexture );
}

// buffer = ARRAY_BUFFER created with glGenBuffers() // openglCreateBuffer()
// ======================================================================== 
void Shader_VertexPointer( GLuint buffer, const int elementSize, const int type, const int stride, const int offset )
{
    // 6.2 No Client Side Arrays
    // INVALID_OPERATION is generated if vertexAttribPointer is called with a WebGLBuffer bound to ARRAY_BUFFER
    oglBindBuffer( GL_ARRAY_BUFFER, buffer );

    // index, size, type, bNormalized, stride, offset
    // type:
    //     GL_BYTE
    //     GL_UNSIGNED_BYTE
    //     GL_SHORT
    //     GL_UNSIGNED_SHORT
    //     GL_INT
    //     GL_UNSIGNED_INT
    //     GL_FLOAT
    //     GL_HALF_FLOAT
    // and GL_
    // Reference:
    //    http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml

    // (intptr_t)
    //    -Wint-to-pointer-cast
    oglVertexAttribPointer(  gpShader->havPosition, elementSize, type, false, stride, (GLvoid*) (intptr_t) offset );
    oglEnableVertexAttribArray( gpShader->havPosition );
}


