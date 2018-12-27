// ========================================================================
void OpenGL_Render()
{
    matrixLoadProj( gmMatrix3D );
    glEnable( GL_DEPTH_TEST );

    // SRT
    // Scale
    // Rotate
    // Translate
    matrixLoadIdentity();
//  matrixTranslate( -gvCameraLocation.x, -gvCameraLocation.y, 0.f );

    // App_Render()

// Render 2D
    matrixLoadProj( gmMatrix2D );
    matrixLoadIdentity();
    glDisable( GL_DEPTH_TEST );

    gMouseCursorSprite.Render();

    // Render FPS
}

// ========================================================================
void OpenGL_Startup()
{
    gnGameMaxTextureDim = 0;
    glGetIntegerv( GL_MAX_TEXTURE_SIZE, &gnGameMaxTextureDim );

    // http://stackoverflow.com/questions/9848901/opengl-use-single-channel-texture-as-alpha-channel-to-display-text
    gnTexturePacking = 0;
    glGetIntegerv( GL_UNPACK_ALIGNMENT, &gnTexturePacking );
    TRACE( "OPENGL: INFO: Texture Packing (default): %d\n", gnTexturePacking );
    // TODO: Set Texture Packing to be 4


    glViewport( 0, 0, gnGameWidth, gnGameHeight );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); // TODO: FIXME: Use pre-multiplied alpha
    // glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );

    glClearDepth( 1.0 );
    glDepthFunc( GL_LESS ); 
    glEnable( GL_DEPTH_TEST );
    glShadeModel( GL_SMOOTH );

    // http://www.opengl.org/sdk/docs/man/xhtml/glFrontFace.xml
    // http://www.opengl.org/sdk/docs/man/xhtml/glCullFace.xml
    glCullFace( GL_BACK ); // default: BACK
    glFrontFace( GL_CCW ); // default:  CCW
    glEnable( GL_CULL_FACE );

    Shader_Startup( true );

    gnGameAspectRatio = (float)gnGameWidth / (float)gnGameHeight;
    gnzNear           = 1.f / 256.f;
    gnzFar            = 32769.f; // 32768 = 32K Draw Distance

    matrixOrtho( gnGameWidth, gnGameHeight );
    matrixCopy( gmMatrix2D, matrixPeekProj() ); // cached orthographic projection

    matrixPerspectiveX( gnGameFieldOfViewX, gnGameAspectRatio, gnzNear, gnzFar );
    matrixCopy( gmMatrix3D, matrixPeekProj() ); // cached perspectiv eprojection

    matrixLoadProj( gmMatrix2D ); // NOTE: Must reload as NOT set above
    matrixLoadIdentity();

    OpenGL_MouseCursor_Startup();
}

// ========================================================================
void OpenGL_Shutdown()
{
    Shader_Shutdown();
}

// ========================================================================
void OpenGL_Update( const float deltaTime )
{
    OpenGL_MouseCursor_Update( deltaTime );
}


