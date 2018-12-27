// = Includes

    #define GAME_NAME "Test Game"

    // Standard
#ifdef _WIN32
    #define _CRT_SECURE_NO_WARNINGS // WIN32:MSVC disable warning C4996: This function or variable may be unsafe.
#endif

    #include <string.h>
    #include <stdint.h>

    #include <sys/types.h> //
    #include <sys/stat.h>  // stat

#ifdef _WIN32
    #include <stdio.h>
    #include <math.h>
    #include <SDL.h>        // includes Windows.h
    #include <SDL_opengl.h> // OutputDebugStringA
    #include <SDL_mixer.h>
#else
    #include <unistd.h>
#ifdef __APPLE__
    #include <SDL.h>
#else // Linux
    #include <SDL2/SDL.h>   // includes: stdio math
#endif // APPLE
#endif // WIN32

    // OpenGL
#ifdef _WIN32
#else
#ifdef __APPLE__
    #define GL_GLEXT_PROTOTYPES 1
    #include <SDL_opengl.h>
#else // Linux
    #include <SDL2/SDL_opengl.h>
#endif // APPLE
#endif // WIN32

// Custom
    #include "debug_trace.h"
    #include "math_util.h" // MATH_PI

// = Globals =

// SDL
    const char   *gpGameTitle        = GAME_NAME;
    int           gnGameWidth        = 1024;
    int           gnGameHeight       =  768;
    int           gnGameWindowX      = -1;
    int           gnGameWindowY      = -1;
    float         gnGameAspectRatio  = 1.0;
    float         gnGameFieldOfViewY = 90.0; // Vertical FOV
    float         gnGameFieldOfViewX = 90.0; // Horizontal FOV

    SDL_Window *  gpGameWindow       = NULL;
    SDL_GLContext gRenderContextGL;

// Game Window
    int           gnGameWindowFlags    = 0;
    bool          gbGameIsFullscreen   = false;
    bool          gbGameIsWindowed     = true;
    int           gnGameVSync          = 1; // ON by default; use command line: -vsync or +vsync or =vsync
    bool          gbGameIsRunning      = true;
    bool          gbGameIsPaused       = false;
    bool          gbGameSingleStep     = false;

// Game Frame
    float         ganFrameClearColor[4]   = { 1.0f, 0.0f, 1.0f, 1.f }; // hot pink

    size_t        gnFrame                 = 0;
    Uint32        gnFrameStart            = 0;
    Uint32        gnFrameEnd              = 0;
    Uint32        gnFrameDelta            = 0; // ms of last frame rendered
    short         ganFrameHistogram[ 120 ];
    bool          gbFrameDebug            = 0;

    Uint32        gnRenderStart           = 0;
    Uint32        gnRenderEnd             = 0;
    int           gnFrameTimeMilliseconds = 0; // time in ms to render this frame
    int           gnFramesPerSecond       = 0; // instantenous

// Game Assets
    char         GAME_PATH_DATA[4096] = ""; // Always ends in /

// OpenGL
    GLint         gnGameMaxTextureDim = 0;
    GLint         gnTexturePacking    = 0;

    #include "opengl_funcs.h"
    #include "opengl_matrix.h"
    #include "opengl_matrixstack.h"
    #include "opengl_shader.h"

    #include "opengl_matrix.cpp"
    #include "opengl_matrixstack.cpp"
    #include "opengl_buffers.cpp"

    #include "opengl_render.cpp"
    #include "opengl_shader.cpp"

// Util
    #include "util_fatal.h"
    #include "util_file.h"
    #include "util_imageinfo.h"
    #include "util_itoa.h"      // itoa_comma()
    #include "util_memory.h"    // swizzle32()
    #include "util_mempool.h"   // _1M

    #include "util_fatal.cpp"
    #include "util_png.cpp"

// ________________________________________________________________________


// ========================================================================
void Game_DataLoad()
{

}

// ========================================================================
void Game_Input()
{
    SDL_Event event;

    while( SDL_PollEvent( &event ) != 0 ) // implicit SDL_PumpEvents()
    {
        if( event.type == SDL_QUIT )
            gbGameIsRunning = false;

        if( event.type == SDL_KEYDOWN )
        {
            int key = event.key.keysym.sym;

            if ( key == SDLK_ESCAPE )
                gbGameIsRunning = false;
        }
    }
}

// ========================================================================
void Game_Options( const int nArg, const char ** aArg )
{
    // Find path to 'data'
    memset( GAME_PATH_DATA, 0, sizeof( GAME_PATH_DATA ) );

    bool isDirectory;
    if( File_Exists( "data", &isDirectory ) )
        strcpy( GAME_PATH_DATA, "data" );
    else
    if( File_Exists( "../data", &isDirectory ) )
        strcpy( GAME_PATH_DATA, "../data" );
    else
    if( File_Exists( "../../data", &isDirectory ) )
        strcpy( GAME_PATH_DATA, "../../data" );

    strcat( GAME_PATH_DATA, "/" );
}

// ========================================================================
void Game_Render()
{
    glClearColor( ganFrameClearColor[0], ganFrameClearColor[1], ganFrameClearColor[2], ganFrameClearColor[3] );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    gnRenderStart = SDL_GetTicks();
        OpenGL_Render();
        SDL_GL_SwapWindow( gpGameWindow );
    gnRenderEnd   = SDL_GetTicks();

    gnFrameTimeMilliseconds = gnRenderEnd - gnRenderStart;
    gnFrameTimeMilliseconds |= (int)(gnFrameTimeMilliseconds == 0); // prevent divide by zero
    gnFramesPerSecond = (1000 / gnFrameTimeMilliseconds);

// Show FPS
    if( gbFrameDebug )
    {
        TRACE( "# %u  %u ms\n", (uint32_t)gnFrame, gnFrameTimeMilliseconds );
    }
    gnFrame++;
}

// ========================================================================
void Game_Shutdown()
{
    MemPoolMgr_Shutdown();
    OpenGL_Shutdown();

    SDL_DestroyWindow( gpGameWindow );
    SDL_Quit();
}

// ========================================================================
bool Game_Startup( int nArg, char *aArg[] )
{
    MemPoolMgr_Startup();

    Game_Options( nArg, (const char**)aArg );

    SDL_DisplayMode aDisplay[ 16 ];
    SDL_DisplayMode tDisplay;
    int             iError;
    int             windowFlags = SDL_WINDOW_OPENGL | gnGameWindowFlags | SDL_RENDERER_PRESENTVSYNC; // | SDL_WINDOW_SHOWN

    // Compiler warns about not being set
    aDisplay[0].w = 0;
    aDisplay[0].h = 0;

    iError = SDL_Init( SDL_INIT_VIDEO );
    if( iError < 0 )
    {
        TRACE( "ERROR: Could not initialize SDL2!   Error: %s\n", SDL_GetError() );
        return 0;
    }

    SDL_Rect bounds;
    int      iDisplay = 0;
    int      nDisplay = SDL_GetNumVideoDisplays();
#if PROFILE || DEBUG
    int      nFoundDisplay    = -1;
#endif
    bool     bFoundResolution = false;

    for( iDisplay = 0; iDisplay < nDisplay; iDisplay++ )
    {
        SDL_GetDisplayBounds( iDisplay, &bounds );
        iError = SDL_GetCurrentDisplayMode( iDisplay, &tDisplay );
        if( iError )
        {
            TRACE( "ERROR: Unable to get Display %d: %s\n", iDisplay, SDL_GetError() );
        }
        else
        {
            if( iDisplay == 0)
                aDisplay[0] = tDisplay;

            // Exclude rotated displays
            // If we only have 1 total displays then INCLUDE rotated displays
            // If we have more then 1 total displays then EXCLUDE them
            bool bDisplayIsRotated = (tDisplay.w < tDisplay.h);
            bool bCheckDisplay = (nDisplay == 1)
            ? true
            : bDisplayIsRotated
                ? false
                : true;

            {
                TRACE( "VIDEO: INFO: Display #%d/%d: %dx%d @ %d Hz   <%4d,%4d> [%4d,%4d]  %s  %s\n",
                    iDisplay+1, nDisplay,
                    tDisplay.w, tDisplay.h, tDisplay.refresh_rate,
                    bounds.x, bounds.y,
                    bounds.w, bounds.h,
                    bDisplayIsRotated
                        ? "Rotated"
                        : "Normal ",
                    bCheckDisplay
                        ? "included"
                        : "excluded"
                );
            }

            if( bCheckDisplay && !bFoundResolution )
            {
                if( (gnGameWidth <= tDisplay.w) && (gnGameHeight <= tDisplay.h) )
                {
#if PROFILE || DEBUG
                    nFoundDisplay    = iDisplay + 1;
#endif
                    bFoundResolution = true;

                    gnGameWindowX = bounds.x;
                    gnGameWindowY = bounds.y;
                    {
                        TRACE( "VIDEO: INFO: monitor %d @ <%d,%d>\n", iDisplay+1, gnGameWindowX, gnGameWindowY );
                    }
                    // Uncomment to first acceptable display
                    break; // default to first display
                }
                else
                if( ((tDisplay.w < gnGameWidth) && (tDisplay.h < gnGameHeight)) || (!gnGameWidth) || (!gnGameHeight) )
                {
                    {
                        TRACE( "VIDEO: INFO: Resizing game window: #%d   From: %d x %d   To: %d x %d\n",
                            iDisplay,
                            gnGameWidth, gnGameHeight,
                            tDisplay.w, tDisplay.h
                        );
                    }
#if PROFILE || DEBUG
                    nFoundDisplay    = iDisplay + 1;
#endif
                    gnGameWidth      = tDisplay.w;
                    gnGameHeight     = tDisplay.h;
                    bFoundResolution = true;
                }
            }
        }
    }

    if( !bFoundResolution || (gnGameWidth == 0) || (gnGameHeight == 0) )
    {
        gnGameWidth  = aDisplay[0].w;
        gnGameHeight = aDisplay[0].h;

        TRACE("VIDEO: INFO: Didn't find screen resolution.  Defaulting to Display #0 %d x %d\n",
            gnGameWidth, gnGameHeight
        );
    }

    // OpenGL settings
    int glMajorActual    = 0;
    int glMinorActual    = 0;
    int glMajorRequested = 0;
    int glMinorRequested = 0;

    iError = 0;
    iError |= SDL_GL_SetAttribute( SDL_GL_RED_SIZE    ,  8 );
    iError |= SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE  ,  8 );
    iError |= SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE   ,  8 );

    iError |= SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE  , 24 );
    iError |= SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE,  8 );

    iError |= SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER      , 1 );
    iError |= SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );

    // Create game window
    if( gbGameIsFullscreen && (nDisplay == 1) )
    {
        gnGameWindowX = SDL_WINDOWPOS_CENTERED;
        gnGameWindowY = SDL_WINDOWPOS_CENTERED;
    } else
    if( bFoundResolution )
    {
        TRACE( "VIDEO: INFO: Using monitor %d at <%d,%d>  %d x %d (found)\n",
            nFoundDisplay,
            bounds.x, bounds.y,
            bounds.w, bounds.h
        );

        if( gnGameWidth  > bounds.w ) gnGameWidth  = bounds.w;
        if( gnGameHeight > bounds.h ) gnGameHeight = bounds.h;

        if( gnGameWindowX < 0 ) gnGameWindowX = bounds.x;
        if( gnGameWindowY < 0 ) gnGameWindowY = bounds.y;
    }

    {
        TRACE( "VIDEO: INFO: Game window: %d,%d @ <%d,%d>\n", gnGameWidth, gnGameHeight, gnGameWindowX, gnGameWindowY );
    }

    gpGameWindow = SDL_CreateWindow( // title, x, y, w, h, flags
          gpGameTitle
        , gnGameWindowX
        , gnGameWindowY
        , gnGameWidth
        , gnGameHeight
        , windowFlags
    );

    if( !gpGameWindow )
    {
        TRACE( "ERROR: SDL2 CreateWindow()\n" );
        return false;
    }

    // SDL_SetWindowDisplayMode( gpGameWindow, NULL ); // NULL = Window Dimensions & Desktop format & refresh

    iError = SDL_GL_GetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, &glMajorActual); if (iError < 0) { TRACE( "OpenGL: WARNING: Unable to get OpenGL Major Version\n" ); }
    iError = SDL_GL_GetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, &glMinorActual); if (iError < 0) { TRACE( "OpenGL: WARNING: Unable to get OpenGL Minor Version\n" ); }

    {
        TRACE( "OpenGL: INFO: Default version: %d.%d\n", glMajorActual, glMinorActual );
    }

    gRenderContextGL = SDL_GL_CreateContext( gpGameWindow );
    if( !gRenderContextGL )
    {
        TRACE( "ERROR: SDL2 OpenGL CreateContext() failed!\n" );
        return false;
    }

    //
    // V-Sync
    //
    // ON  +prevent screen tearing, -has input lag
    // OFF -has screen tearing    , +no input lag
    {
        TRACE( "  -> %dx%d @ %d Hz   V-Sync: %d (requested)\n", gnGameWidth, gnGameHeight, tDisplay.refresh_rate, gnGameVSync );
    }

    //gnGameVSync=0; // DEBUG/TEST: Force V-Sync Off

    // SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1); // SDL 1.2
    iError =  SDL_GL_SetSwapInterval( gnGameVSync ); // -1
    if( iError < 0 )
    {
        TRACE( "VIDEO: WARNING: Unable to set V-Sync %d\n", gnGameVSync );
    }

    // OpenGL info
    {
        TRACE( "= OpenGL =\n" );
        TRACE( "  Version   : %s\n", glGetString( GL_VERSION    ) );
        TRACE( "  Vendor    : %s\n", glGetString( GL_VENDOR     ) );
        TRACE( "  Renderer  : %s\n", glGetString( GL_RENDERER   ) );
        TRACE( "  Extensions: %s\n", glGetString( GL_EXTENSIONS ) );
    }

    SDL_GL_GetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, &glMajorRequested );
    SDL_GL_GetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, &glMinorRequested );

    {
        TRACE( "OPENGL: INFO: Detected OpenGL version: %d.%d\n", glMajorRequested, glMinorRequested );
    }

    gnGameMaxTextureDim = 0;
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &gnGameMaxTextureDim );

    // http://stackoverflow.com/questions/9848901/opengl-use-single-channel-texture-as-alpha-channel-to-display-text
    gnTexturePacking = 0;
    glGetIntegerv( GL_UNPACK_ALIGNMENT, &gnTexturePacking );
    TRACE( "OPENGL: INFO: Texture Packing (default): %d\n", gnTexturePacking );
    // TODO: Set Texture Packing to be 4

    OpenGL_Startup();

    Game_DataLoad();

    return true;
}

// ========================================================================
bool Game_Run()
{
    if( gbGameIsRunning ) //&& gGameState != WORLD_STATE_LOADING)
    {
        gnFrameEnd   = SDL_GetTicks();
        gnFrameDelta = gnFrameEnd - gnFrameStart; // ms
        gnFrameStart = gnFrameEnd;

        if( gnFrameDelta <   3 )
            gnFrameDelta =   3 ; // don't run faster then 300 fps (1000ms/s /  3ms/frame)
        if( gnFrameDelta > 120 )
            gnFrameDelta = 120 ; // don't run slower then   8 fps (1000ms/s /120ms/frame) (i.e. when Real-Time Debugging)

        Game_Input();
        // Game_Update( gnFrameDelta * 1.f / 1000.f ); // seconds
        Game_Render();
    }

    return gbGameIsRunning;
}

// ========================================================================
int main(  int nArg, char* aArg[] )
{
    Game_Startup( nArg, aArg );

    while( Game_Run() )
        ;

    Game_Shutdown();

    return 0;
}

