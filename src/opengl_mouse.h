// Mouse
    int          gnMouseXnext      = 0; // prev mouse coordinates constrained to game window
    int          gnMouseYnext      = 0;
    int          gnMouseXprev      = 0;
    int          gnMouseYprev      = 0;
    int          gnMouseStopTimeMS = 0; // milli-seconds

void OpenGL_MouseCursor_Startup();
void OpenGL_MouseCursor_Update( const float deltaTime );

