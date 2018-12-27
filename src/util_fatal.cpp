// always returns bValid = false
// ========================================================================
bool Game_Fatal( const char *format, const char *text /* = NULL*/ )
{
    gbGameIsRunning = false;

    char message[ 512 ];
    if( text )
        sprintf( message, "FATAL: %s%s", format, text );
    else
        sprintf( message, "FATAL: %s", format );

    SDL_ShowSimpleMessageBox( SDL_MESSAGEBOX_ERROR, "Something went wrong ...", message, NULL );

    return false;
}

