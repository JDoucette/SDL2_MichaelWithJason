// = Includes

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


// ======================================================================== 
int main(  int nArg, char* aArg[] )
{
    printf( "Hello World\n" );

    return 0;
}

