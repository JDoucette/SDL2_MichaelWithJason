    Sprite2D gMouseCursorSprite;

// ========================================================================
void OpenGL_MouseCursor_Startup()
{
    TRACE( "OpenGL_MouseCursor_Startup() Loading mouse cursor ...\n" );

    gMouseCursorSprite.Load( "sprites/cursor/mouse.png", 0, 1 );
    gMouseCursorSprite.FlipX(false);
    gMouseCursorSprite.SetFlipReversed();
    gMouseCursorSprite.BindTexture();
    gMouseCursorSprite.speed = 0;
    gMouseCursorSprite.Animate(false, 0.1f, 0, 1);

    gMouseCursorSprite.vLocation.x = (float) gnMouseXnext; // Screen Space
    gMouseCursorSprite.vLocation.y = (float) gnMouseYnext; // Screen Space

    gMouseCursorSprite.Show();
}

// ========================================================================
void OpenGL_MouseCursor_Update( const float deltaTime )
{
    (void) deltaTime;

    // move the mouse to it's position
    gMouseCursorSprite.vLocation.x = (float) gnMouseXnext;
    gMouseCursorSprite.vLocation.y = (float) gnMouseYnext;

//printf( "Mouse.x: %f\n", gMouseCursorSprite.vLocation.x );
//printf( "Mouse.y: %f\n", gMouseCursorSprite.vLocation.y );

/*
    // testing the buttons: just flip the cursor if the left button is pressed
    if (gaMouseButtons[SDL_BUTTON_LEFT] == 1)
    {
        gMouseCursorSprite.FlipX(false);
    }
    else
    {
        gMouseCursorSprite.FlipX(true);
    }
*/
    gMouseCursorSprite.Update(1.0f);
}

void OpenGL_MouseCursor_Render()
{
}

