/*
OpenGL utility matrix functions that were removed in OpenGL 3+ gl.h and glu.h
*/

// = Macros =
    #define ORTHO_BOT_LEFT 0

// = Types =

// = Globals = 
    
// = Prototypes =
        void  openglGetDOF ( const float mProjection[16], float & x_, float & y_, float & z_ );
        void  openglFrustum      ( float mProjection[16], const float left, const float right, const float bottom, const float top, const float zNear, const float zFar ); // WIN32
inline  float openglGetFovX( const float mProjection[16] ) { return 360.f * atanf(1.f / mProjection[0]) / MATH_PI; } // 2*atan(#) * 180/PI_deg/rad = 360 * atan(#) / PI
inline  float openglGetFovY( const float mProjection[16] ) { return 360.f * atanf(1.f / mProjection[5]) / MATH_PI; } // 2*atan(#) * 180/PI_deg/rad = 360 * atan(#) / PI
        void  openglOrtho        ( float mProjection[16], const float left, const float right, const float bottom, const float top, const float zNear, const float zFar ); // WIN32
        void  openglOrtho        ( float mProjection[16], const int width, const int height );
        void  openglPerspectiveX ( float mProjection[16], const float fovXdegrees, const float aspectRatio, const float zNear, const float zFar );
        void  openglPerspectiveY ( float mProjection[16], const float fovYdegrees, const float aspectRatio, const float zNear, const float zFar );
