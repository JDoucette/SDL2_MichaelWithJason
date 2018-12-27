// = Macros =
    #if DEBUG || PROFILE
        #undef TRACE
        #ifdef OS_WIN
            #define TRACE(...) debugTrace(##__VA_ARGS__)
        #else
            #define TRACE(...) debugTrace(__VA_ARGS__)
        #endif
    #else
        #define TRACE(...)
    #endif


// = Prototypes =
	// source: http://stackoverflow.com/questions/679979/how-to-make-a-variadic-macro-variable-number-of-arguments
	void debugTrace( const char *format, ... );


// = Implementation =
#if DEBUG || PROFILE
    // NOTE: Requires <windows.h> included via: <SDL_opengl.h>
    void debugTrace( const char *format, ... )
    {
        char buffer[ 8192 ]; // TODO: static?

        va_list args;
            va_start( args, format );
            vsprintf( buffer, format, args );
        va_end( args );

#ifdef OS_WIN
        OutputDebugStringA(buffer);
#else
        printf("%s", buffer);
#endif // OS_WIN
    }
#endif // DEBUG PROFILE

