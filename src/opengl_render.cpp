void OpenGL_Render()
{
//    matrixLoadProj( gmMatrix3D );
    glEnable( GL_DEPTH_TEST );

    // SRT
    // Scale
    // Rotate
    // Translate
//    matrixLoadIdentity();
//   matrixTranslate( -gvCameraLocation.x, -gvCameraLocation.y, 0.f );

//    World_Render();

// Render 2D
//  matrixLoadProj( gmMatrix2D );
//  matrixLoadIdentity();
    glDisable( GL_DEPTH_TEST ); // "cheap" glClear GL_DEPTH_BUFFER_BIT
}

void OpenGL_Startup()
{
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
}

void OpenGL_Shutdown()
{
    Shader_Shutdown();
}

