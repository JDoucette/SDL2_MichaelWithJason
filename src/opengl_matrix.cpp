/*
OpenGL utility matrix functions that were removed in OpenGL 3+ gl.h and glu.h
*/

// = Includes =
    
// = Implementation =

void openglGetDOF( const float mView[16], float & x_, float & y_, float & z_ )
{
#if 1
    x_ = mView[  2 ];
    y_ = mView[  6 ]; 
    z_ = mView[ 10 ];
#else
    x_ = mView[  8 ];
    y_ = mView[  9 ]; 
    z_ = mView[ 10 ];
#endif
}

// ======================================================================== 
void openglFrustum( float mProjection[16], 
    const float nLeft  , const float nRight, 
    const float nBottom, const float nTop  ,
    const float nNear  , const float nFar  )
{
    /* */ float *m =&mProjection[0];
    const float dx = nRight - nLeft  ; // difference
    const float dy = nTop   - nBottom;
    const float dz = nFar   - nNear  ;
    const float sx = nRight + nLeft  ; // sum
    const float sy = nTop   + nBottom;
    const float sz = nFar   + nNear  ;
    const float n2 = 2.f    * nNear  ; // common factor: 2 * zNear

    m[0] = n2 / dx; m[4] = 0.f    ; m[ 8] =  sx / dx; m[12] = 0.f            ;
    m[1] = 0.f    ; m[5] = n2 / dy; m[ 9] =  sy / dy; m[13] = 0.f            ;
    m[2] = 0.f    ; m[6] = 0.f    ; m[10] = -sz / dz; m[14] = n2 * -nFar / dz;
    m[3] = 0.f    ; m[7] = 0.f    ; m[11] = -1.f    ; m[15] = 0.f            ;
}


// Given n = m[0] or m[5], returns the FOV in degrees
float openglGetFovDegrees( const float n )
{
    return 360.f * atanf( 1.f / n ) / MATH_PI; // fov/2 * 180/PI
}

/*
Theorem: 
   Given a projection matrix in the form:
      [p 0 ? 0]
      [0 q ? 0]
      [0 0 ? ?]
      [0 0 ? ?]

   Then
      p = q * aspectRatio = 1 / Tan( fovY/2 )
      q = p / aspectRatio = 1 / Tan( fovX/2 )

   And the reciprocals are:
      fovY = 2 * aTan( 1/p )
      fovX = 2 * aTan( 1/q )

Proof:
    The projection matrix was orginally calculated in PerspectiveY()
       // Optimization Degrees to Radians: fovY/2 * PI/180 = fovY * PI/360
       yMax =  zNear * Tan( fovY/2 )
       yMin = -yMax

   And called Frustum() with parameters:
      Bottom = yMin, Top = yMax

   Which set m[5]:
      m[5] = 2*zNear / (Top  - Bottom)

   Simplifying:
           = 2*zNear / (yMax - yMin)
           = 2*zNear / (zNear*Tan(fovY/2) - (-zNear*Tan(fovY/2)))
           = 2*zNear / (zNear*Tan(fovY/2) + zNear*Tan(fovY/2))     // Algebra: z*t + z*t = 2*z*t
           = 2*zNear / 2*zNear*Tan(fovY/2)                         // Algebra: 2*zNear/2*zNear = 1/1
->    p    = 1 / Tan( fovY/2 )

   Solving for fovY:
       p = 1 / Tan( fovY/2 )
       Tan( fovY/2) = 1/p
       aTan( Tan( fovY/2 ) ) = aTan( 1/p )
       fovY/2 = aTan( 1/p )
->     fovY = 2 * aTan( 1/p )

   Similiarly for FovX,

   The projection matrix was orginally calculated in PerspectiveY()
      xMax = yMax * aspectRatio;
      xMin = yMin * aspectRatio;

   And called Frustum() with parameters:
      Left = xMin, Right = xMax

   Which set m[0]:
      m[0] = 2*zNear / (Right - Left)

   Simplifying:
           = 2*zNear / ( xMax - xMin )
           = 2*zNear / ( yMax*aspectRatio    -  yMin*aspectRatio )
           = 2*zNear / ( yMax*aspectRatio    - -yMax*aspectRatio )
           = 2*zNear / ( yMax*aspectRatio    +  yMax*aspectRatio )
           = 2*zNear / ( 2*yMax              *  aspectRatio      )
           = zNear   / ( yMax                *  aspectRatio      )
           = zNear   / ( zNear*Tan( fovY/2 ) *  aspectRatio      )
           = 1       / ( Tan(fovY/2)         *  aspectRatio      )
     ->  q = p/aspectRatio


   Definition of right angle triangle:
      Tan(x) = y / x
          x  = atan( y / x )

   Given a near plane of pixel dimensions width, and height:
   
      Side                  .
      View                / |   ^  
                        /   |   |                          height/2
                      /     | height/2    Tan( fovY/2 ) =  --------
                    /       |   |                           zNear
                  /\ fovY/2 |   |
                /___|_______|   V
      Camera.z    <- zNear ->

      Top    <------- width ------->
      Veiw   ._____________________. ^
              \         |         /  |
               \        |        /   |
                \       |       /    |
                 \      |fovX  /     |                    width/2
                  \     |---- /      |    Tan( fovX/2 ) = -------
                   \    |__2 /       |                     zNear
                    \   |  \/        |zNear
                     \  |  /         |
                      \ | /          |
                       \|/           |
                        . Camera.z   v
   Then
      Tan( fovY/2 ) = height/2 / zNear   &   Tan( fovX/2 ) = width/2 / zNear
      Tan( fovY/2 ) = height / (2*zNear) &   Tan( fovX/2 ) = width / (2*zNear)
      2*zNear = height / Tan( fovY/2 )       2*zNear = width / Tan( fovX/2 )

                   height / Tan( fovY/2) = width / Tan( fovX/2 )
   Simplifying:
      height * p = width / Tan( fovX/2 )
      p = (width/height) / Tan( fovX/2 )
      Tan( fovX/2 ) = (width/height) / p
      Tan( fovX/2 ) = aspectRato / p
      aTan( Tan( fovX/2 ) = aTan( aspectRatio / p )
      fovX / 2 = aTan( aspectRatio / p )
      fovX = 2 * aTan( aspectRatio / p )
->    fovX = 2 * aTan( 1/q )                 // q = p/aspectRatio   1/q = aspectRatio/p
*/

// @param matrix PerspectiveY fov matrix
// ======================================================================== 
/*
inline
float openglGetFovX( float const mProjection[16] )
{
    return 360.f * atanf(1. / mProjection[0]) / MATH_PI; // 2*atan(#) * 180/PI_deg/rad = 360 * atan(#) / PI
}
*/

// @param matrix PerspectiveX fov matrix
// ======================================================================== 
/* inline
float openglGetFovY( float const mProjection[16] )
{
    return 360.f * atanf(1. / mProjection[5]) / MATH_PI; // 2*atan(#) * 180/PI_deg/rad = 360 * atan(#) / PI
}
*/


/*
Q. Which is the correct order: glOrtho(  0,w,0,h ) or glOrtho( 0,w,h,0 ) ?
A. The order determines whether the origin is in the bottom left or top right.

First, let's analyze the default orthographic projection with the identity matrix.
We know that glOrtho calculates this matrix with the origin in the bottom left:

    [1 0 0 0]    [ 2/dx  0            -sx/dx ]
    [0 1 0 0] -> [ 0     2/dy         -sy/dy ]
    [0 0 1 0]    [ 0     1     -2/dz  -sz/dz ]
    [0 0 0 1]    [ 0     0             1     ]

Where:
   dx = right - left     sx = right + left
   dy = top   - bottom   sy = top   + bottom
   dz = far   - near     sz = far   + near

Solving for right and left:
  1 = 2/2
      2/dx
      2/(right - left)
      2/(+1 - -1)

Solving for top and bottom:
  1 = 2/2
      2/dy
      2/(top - bottom)
      2/(+1 - -1)

Solving for near and far:
  1 = -2/-2
      -2/dz
      -2/(far - near)
      -2/(-1 - +1)

Analyze glOrtho( 0, w, 0, h ): Top = +1, Bot = -1
   dy = top - bottom
      = 1 - -1
      = 2
      Origin Bottom Left, default winding

Analyze glOrtho( 0, w, h, 0 ): Top = -1, Bot = +1
   dy = top - bot
      = -1 - +1
      = -2
      Origin Top Left, reverse winding or glDisable(GL_CULL_FACE);
*/
// @return Orthographic projection matrix with <0,0> top left and <width-1, height-1> bottom right. Z Near = 0, Z Far = 1.
// ======================================================================== 
void openglOrtho( float mProjection[16], const int width, const int height )
{
    // TODO: FIXME: Fence-post error? Are these supposed to be 0,w-1,0,h-1 or 0,w,0,h
    const float w = (float)width ;
    const float h = (float)height;

#if ORTHO_BOT_LEFT // OpenGL Orthographic Origin Bottom Left
    openglOrtho( mProjection,
         0.f, w  , // l, r 
         0.f, h  , // b, t // Origin: Bottom Left
        -1.f, 1.f  // n, f
    );
#else
    openglOrtho( mProjection,
         0.f, w  , // l, r 
         h  , 0.f, // b, t // Origin: Top Left - texture upside down, reverse CULL Winding Order
        -1.f, 1.f  // n, f
    );
#endif
}

// ======================================================================== 
void  openglOrtho( float mProjection[16],
    const float nLeft  , const float nRight ,
    const float nBottom, const float nTop   ,
    const float nNear  , const float nFar   )
{
    
    /* */ float *m = &mProjection[0];
    const float dx = nRight - nLeft  ; // width // difference
    const float dy = nTop   - nBottom; // height
    const float dz = nFar   - nNear  ;
    const float sx = nRight + nLeft  ; // sum
    const float sy = nTop   + nBottom;
    const float sz = nFar   + nNear  ;

    m[0] = 2.f / dx; m[4] = 0.f     ; m[ 8] =  0.f     ; m[12] = -sx / dx;
    m[1] = 0.f     ; m[5] = 2.f / dy; m[ 9] =  0.f     ; m[13] = -sy / dy;
    m[2] = 0.f     ; m[6] = 0.f     ; m[10] = -2.f / dz; m[14] = -sz / dz;
    m[3] = 0.f     ; m[7] = 0.f     ; m[11] =  0.f     ; m[15] = 1.f;
}


// ======================================================================== 
// @param aspectRatio width/height
void openglPerspectiveX( float mProjection[16], const float fovXdegrees, const float aspectRatio, const float zNear, const float zFar )
{
    const float xMax = zNear * tanf( fovXdegrees * MATH_PI / 360.f ); // fov/2 * PI/180 = fov*pi/360 
    const float xMin =-xMax              ;
    const float yMax = xMax / aspectRatio;
    const float yMin = xMin / aspectRatio;
    openglFrustum( mProjection, xMin, xMax, yMin, yMax, zNear, zFar );
}


/*
   This is a direct replacement for gluPerspective()

   There are many ways of calculating the projection matrix:
      [ p 0  0 0 ]
      [ 0 q  0 0 ]
      [ 0 0  r s ]
      [ 0 0 -1 0 ]
   Where
      p = q / aspectRatio
      q = 1 / tan( fovY/2 )
      r = (zNear + zFar)   / (zNear - zFar)
      s = 2 * zNear * zFar / (zNear - zFar)
*/
// ======================================================================== 
// @param aspectRatio width/height
void openglPerspectiveY( float mProjection[16], const float fovYdegrees, const float aspectRatio, const float zNear, const float zFar )
{
    const float yMax = zNear * tanf( fovYdegrees * MATH_PI / 360.f ); // fov/2 * PI/180 = fov*pi/360
    const float yMin =-yMax              ;
    const float xMax = yMax * aspectRatio;
    const float xMin = yMin * aspectRatio;
    openglFrustum( mProjection, xMin, xMax, yMin, yMax, zNear, zFar );
}

