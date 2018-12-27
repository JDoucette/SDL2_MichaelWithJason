// Input - Keyboard
    int           gnKeyboardState = 0;
    uint8_t      *gpKeyboardState = NULL;
    uint8_t       gaKeyboardStateThisFrame[ SDL_NUM_SCANCODES ];
    uint8_t       gaKeyboardStatePrevFrame[ SDL_NUM_SCANCODES ];

// ========================================================================
    inline bool Keyboard_IsDown(     SDL_Scancode key ) { return   gaKeyboardStateThisFrame[ key ] ? true : false; }
    inline bool Keyboard_IsUp(       SDL_Scancode key ) { return  !gaKeyboardStateThisFrame[ key ] ? true : false; }
    inline bool Keyboard_IsPressed(  SDL_Scancode key ) { return ( gaKeyboardStateThisFrame[ key ] && !gaKeyboardStatePrevFrame[ key ]) ? true : false; }
    inline bool Keyboard_WasPressed( SDL_Scancode key ) { return (!gaKeyboardStateThisFrame[ key ] &&  gaKeyboardStatePrevFrame[ key ]) ? true : false; }
    inline bool Keyboard_StillDown(  SDL_Scancode key ) { return ( gaKeyboardStateThisFrame[ key ] &&  gaKeyboardStatePrevFrame[ key ]) ? true : false; }
    inline bool Keyboard_StillUp(    SDL_Scancode key ) { return (!gaKeyboardStateThisFrame[ key ] && !gaKeyboardStatePrevFrame[ key ]) ? true : false; }

