// = Includes =

// = Implementation =

// OpenGL 3.1 GL_UNIFORM_BUFFER GL_TEXTURE_BUFFER
// OpenGL 4.2 GL_ATOMIC_COUNTER_BUFFER
// OpenGL 4.3 GL_DISPATCH_INDIRECT_BUFFER GL_SHADER_STORAGE_BUFFER
// OpenGL 4.4 GL_QUERY_BUFFER
// ======================================================================== 
GLuint OpenglCreateBuffer()
{
    GLuint buffer;
    oglGenBuffers( 1, &buffer );
    return buffer;
}

void OpenglDeleteBuffer( GLuint buffer )
{
    GLuint aBuffer = buffer;
    oglDeleteBuffers( 1, &aBuffer );
}

/*
	const float aVertices[] = {
        0.f, 0.f, // Top Left
        w  , 0.f, // Top Right
        0.f, h  , // Bot Left
        w  , h    // Bot Right
    };

    const float aTexCoord[] = {
        0.f, 0.f, // Top Left
        1.f, 0.f, // Top Right
        0.f, 1.f, // Bot Left
        1.f, 1.f  // Bot Right
    };

void
OpenGLCreateBufferXYUV( const GLuint& vBuffer_, const GLuint& tBuffer_
{
if( vBuffer_ != 0 )
   TRACE( "ERROR: Already have Vertices Buffer!" );
if( tBuffer_ != 0 )
   TRACE( "ERROR: Already have TexCoord Buffer!" );

    vBuffer_ = OpenglBufferData( 0, aVertices, sizeof( aVertices ), GL_STATIC_DRAW );
    tBuffer_ = OpenglBufferData( 0, aTexCoord, sizeof( aTexCoord ), GL_STATIC_DRAW );
}
*/


// i.e. 
//    const int elements    = 8;
//    const int sizeofType  = sizeof( float );
//    const int bytes       = elements * sizeofType
//
//    const float aVertices[] = { 0, 0,   w, 0,   0, h,   w, h };
//    const flaot aTexCoord[] = { 0, 0,   1, 0,   0, 1,   1, 1, };
//    GLuint vBuffer = openglBufferData( 0, aVertices, bytes );
//    GLuint tBuffer = openglBufferData( 0, aTexCoord, bytes );
// ======================================================================== 
GLuint OpenglBufferData( const GLuint buffer, const void * data, const int sizeTotalBytes, const int usage )
{
    GLuint iBuffer = buffer;
    if( iBuffer <= 0)
        iBuffer = OpenglCreateBuffer();

    oglBindBuffer( GL_ARRAY_BUFFER, iBuffer );

    // target, size, data, usage
    // target: 2.x GL_ARRAY_BUFFER
    //         4.2 GL_ATOMIC_COUNTER_BUFFER
    //         4.3 GL_SHADERS_STORAGE_BUFFER
    //         4.4 GL_QUERY_BUFFER
    // usage: GL_STATIC_DRAW or GL_DYNAMIC_DRAW or GL_STREAM_DRAW
    // Reference:
    //    http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
    oglBufferDataARB( GL_ARRAY_BUFFER, sizeTotalBytes, data, usage );

    return iBuffer;
}

// ======================================================================== 
void OpenglBufferUpdateData( const GLuint buffer, const void * data, const int sizeTotalBytes, const int offset )
{
    oglBindBuffer( GL_ARRAY_BUFFER, buffer );
    oglBufferSubDataARB( GL_ARRAY_BUFFER, offset, sizeTotalBytes, data );
}

