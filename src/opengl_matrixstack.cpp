// Rotate and Translate is hard-coded to modelview, perspective hard-coded to projection

// = Includes =

// = Types =

// = Consts =
    const char *gaMatrixNames[ NUM_MATRIX_STACKS ] =
    {
         "mView"
        ,"mProj"
        ,"mText"
    };

    const float MAT4_IDENTITY[16] =
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

// = Globals =

    //  Value
    float gaMatrixView[ MATRIXSTACK_MAX_DEPTH ][16];
    float gaMatrixProj[ 4                     ][16];
    float gaMatrixText[ 4                     ][16];

    // Depth
    int  gnMatrixView = 0;
    int  gnMatrixProj = 0;
    int  gnMatrixText = 0;

// = Implementation =


// ========================================================================
/*
inline void
matrixCopy( float *pDst, const float *pSrc )
{
    const size_t nBytes = sizeof( float ) * 16;
    memcpy( pDst, pSrc, nBytes );
}
*/


/*
// ========================================================================
inline void
matrixLoadIdentity()
{
    matrixCopy( &gaMatrixView[ gnMatrixView ][0], &MAT4_IDENTITY[0] );
}
*/


// ========================================================================
inline void
matrixMultiply( float dst[16], const float lhs[16], const float rhs[16] )
{
    const float a0 = lhs[ 0];
    const float a1 = lhs[ 1];
    const float a2 = lhs[ 2];
    const float a3 = lhs[ 3];
    const float a4 = lhs[ 4];
    const float a5 = lhs[ 5];
    const float a6 = lhs[ 6];
    const float a7 = lhs[ 7];
    const float a8 = lhs[ 8];
    const float a9 = lhs[ 9];
    const float aA = lhs[10];
    const float aB = lhs[11];
    const float aC = lhs[12];
    const float aD = lhs[13];
    const float aE = lhs[14];
    const float aF = lhs[15];

    const float b0 = rhs[ 0];
    const float b1 = rhs[ 1];
    const float b2 = rhs[ 2];
    const float b3 = rhs[ 3];
    const float b4 = rhs[ 4];
    const float b5 = rhs[ 5];
    const float b6 = rhs[ 6];
    const float b7 = rhs[ 7];
    const float b8 = rhs[ 8];
    const float b9 = rhs[ 9];
    const float bA = rhs[10];
    const float bB = rhs[11];
    const float bC = rhs[12];
    const float bD = rhs[13];
    const float bE = rhs[14];
    const float bF = rhs[15];

    // dst[y][x] = dot( lhs.row(y), rhs.col(x) )
    dst[ 0] = a0*b0 + a4*b1 + a8*b2 + aC*b3;
    dst[ 1] = a1*b0 + a5*b1 + a9*b2 + aD*b3;
    dst[ 2] = a2*b0 + a6*b1 + aA*b2 + aE*b3;
    dst[ 3] = a3*b0 + a7*b1 + aB*b2 + aF*b3;

    dst[ 4] = a0*b4 + a4*b5 + a8*b6 + aC*b7;
    dst[ 5] = a1*b4 + a5*b5 + a9*b6 + aD*b7;
    dst[ 6] = a2*b4 + a6*b5 + aA*b6 + aE*b7;
    dst[ 7] = a3*b4 + a7*b5 + aB*b6 + aF*b7;

    dst[ 8] = a0*b8 + a4*b9 + a8*bA + aC*bB;
    dst[ 9] = a1*b8 + a5*b9 + a9*bA + aD*bB;
    dst[10] = a2*b8 + a6*b9 + aA*bA + aE*bB;
    dst[11] = a3*b8 + a7*b9 + aB*bA + aF*bB;

    dst[12] = a0*bC + a4*bD + a8*bE + aC*bF;
    dst[13] = a1*bC + a5*bD + a9*bE + aD*bF;
    dst[14] = a2*bC + a6*bD + aA*bE + aE*bF;
    dst[11] = a3*bC + a7*bD + aB*bE + aF*bF;
}


// ========================================================================
inline void
matrixMultiplyRotation( float dst[16], const float lhs[16], const float rhs[16] )
{
    const float a0 = lhs[ 0];
    const float a1 = lhs[ 1];
    const float a2 = lhs[ 2];
    const float a3 = lhs[ 3];
    const float a4 = lhs[ 4];
    const float a5 = lhs[ 5];
    const float a6 = lhs[ 6];
    const float a7 = lhs[ 7];
    const float a8 = lhs[ 8];
    const float a9 = lhs[ 9];
    const float aA = lhs[10];
    const float aB = lhs[11];
    const float aC = lhs[12];
    const float aD = lhs[13];
    const float aE = lhs[14];
    const float aF = lhs[15];

    const float b0 = rhs[ 0];
    const float b1 = rhs[ 1];
    const float b2 = rhs[ 2];
//  const float b3 = rhs[ 3]; // 0
    const float b4 = rhs[ 4];
    const float b5 = rhs[ 5];
    const float b6 = rhs[ 6];
//  const float b7 = rhs[ 7]; // 0
    const float b8 = rhs[ 8];
    const float b9 = rhs[ 9];
    const float bA = rhs[10];
//  const float bB = rhs[11]; // 0
//  const float bF = rhs[15]; // 1

    // dst[y][x] = dot( lhs.row(y), rhs.col(x) )
    dst[ 0] = a0*b0 + a4*b1 + a8*b2     ; //+ aC*b3;
    dst[ 1] = a1*b0 + a5*b1 + a9*b2     ; //+ aD*b3;
    dst[ 2] = a2*b0 + a6*b1 + aA*b2     ; //+ aE*b3;
    dst[ 3] = a3*b0 + a7*b1 + aB*b2     ; //+ aF*b3;

    dst[ 4] = a0*b4 + a4*b5 + a8*b6     ; //+ aC*b7;
    dst[ 5] = a1*b4 + a5*b5 + a9*b6     ; //+ aD*b7;
    dst[ 6] = a2*b4 + a6*b5 + aA*b6     ; //+ aE*b7;
    dst[ 7] = a3*b4 + a7*b5 + aB*b6     ; //+ aF*b7;

    dst[ 8] = a0*b8 + a4*b9 + a8*bA     ;
    dst[ 9] = a1*b8 + a5*b9 + a9*bA     ;
    dst[10] = a2*b8 + a6*b9 + aA*bA     ;
    dst[11] = a3*b8 + a7*b9 + aB*bA     ;

    dst[12] =                         aC;
    dst[13] =                         aD;
    dst[14] =                         aE;
    dst[15] =                         aF;
}


// ========================================================================
void
matrixOrtho( const int screenWidth, const int screenHeight )
{
    float *pProjectionX = &gaMatrixProj[ gnMatrixProj ][0];
    openglOrtho( pProjectionX, screenWidth, screenHeight );
}


/*
// ========================================================================
inline
const float* matrixPeekView()
{
    return &gaMatrixView[ gnMatrixView ][0];
}


// ========================================================================
inline
const float* matrixPeekProj()
{
    return &gaMatrixProj[ gnMatrixProj ][0];
}
*/


// ========================================================================
void
matrixPerspectiveX( const float fovXdegrees, const float aspectRatio, const float zNear, const float zFar )
{
 //   float *pProjectionX = &gaMatrixProj[ gnMatrixProj ][0];
    openglPerspectiveX( gaMatrixProj[ gnMatrixProj ], fovXdegrees, aspectRatio, zNear, zFar );
}


// ========================================================================
void
matrixPerspectiveY( const float fovXdegrees, const float aspectRatio, const float zNear, const float zFar )
{
//    float *pProjectionY = &gaMatrixProj[ gnMatrixProj ][0];
    openglPerspectiveY( gaMatrixProj[ gnMatrixProj ], fovXdegrees, aspectRatio, zNear, zFar );
}


// ========================================================================
void
matrixPrint( const float *m, const char *pHeaderText /* = 0 */ )
{
    (void) m;

    if( pHeaderText )
    {
        TRACE( "%s\n", pHeaderText );
    }
    TRACE( "[ %+5.3f %+5.3f %+5.3f %+5.3f ]\n", m[0], m[4], m[ 8], m[12] );
    TRACE( "[ %+5.3f %+5.3f %+5.3f %+5.3f ]\n", m[1], m[5], m[ 9], m[13] );
    TRACE( "[ %+5.3f %+5.3f %+5.3f %+5.3f ]\n", m[2], m[6], m[10], m[14] );
    TRACE( "[ %+5.3f %+5.3f %+5.3f %+5.3f ]\n", m[3], m[7], m[11], m[15] );
}


// Duplicate top MODELVIEW matrix
// ========================================================================
void
matrixPush()
{
    const float *pSrc = &gaMatrixView[ gnMatrixView+0 ][0];
    /* */ float *pDst = &gaMatrixView[ gnMatrixView+1 ][0];
    matrixCopy( pDst, pSrc );

    if( gnMatrixView < MATRIXSTACK_MAX_DEPTH )
        gnMatrixView++;
    else
    {
        TRACE( "ERROR: Matrix %s max depth!\n", gaMatrixNames[ MATRIX_VIEW ] );
    }
}


// ========================================================================
void
matrixPushTranslateXY( const float x, const float y )
{
    const float *pSrc = &gaMatrixView[ gnMatrixView+0 ][0];
    /* */ float *pDst = &gaMatrixView[ gnMatrixView+1 ][0];
    matrixCopy( pDst, pSrc );
    gnMatrixView++;

    pDst[ 12 ] += x;
    pDst[ 13 ] += y;
}


// Remove top MODELVIEW matrix
// ========================================================================
inline void
matrixPop()
{
    if( gnMatrixView > 0 )
        gnMatrixView--;
}


// Reference: http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/index.htm
// ========================================================================
void
matrixRotate( const float angleDegrees, float axisX, float axisY, float axisZ )
{
    const int    i = gnMatrixView        ; // depth
    /* */ float *u =&gaMatrixView[i+0][0]; // src
    /* */ float *v =&gaMatrixView[i+1][0]; // dst
    /* */ float *w =&gaMatrixView[i+2][0]; // tmp
    matrixCopy( v, u );

    const float  r = DEG_TO_RAD( angleDegrees );
    const float  c = cosf( r );
    const float  s = sinf( r );
    const float  m = 1.f - c;
    const float  d = sqrtf( axisX*axisX + axisY*axisY + axisZ*axisZ );
    const float rd = 1.f / d;
    const float  x = axisX * rd;
    const float  y = axisY * rd;
    const float  z = axisZ * rd;

    const float xx = x*x;
    const float yy = y*y;
    const float zz = z*z;

    const float xy = x*y;
    const float yz = y*z;
    const float zx = z*x;

    const float xs = x*s;
    const float ys = y*s;
    const float zs = z*s;
#if 1
    w[ 0] = m*xx + c ;
    w[ 1] = m*xy + zs;
    w[ 2] = m*zx - ys;
//  w[ 3] = 0.f;

    w[ 4] = m*xy - zs;
    w[ 5] = m*yy + c ;
    w[ 6] = m*yz + xs;
//  w[ 7] = 0.f;

    w[ 8] = m*zx + ys;
    w[ 9] = m*yz - xs;
    w[10] = m*zz + c ;
//  w[11] = 0.f;
#else // equivalent

   /*    [ 1 0 0 ]       [ x*x  x*y  x*z  ]     [  0  -z   y ]
       c [ 0 1 0 ] + m * [ x*y  y*y  y*z  ] + s [  z   0  -x ]
         [ 0 0 1 ]       [ x*z  y*z  z*z  ]     [ -y  -x   0 ]
   */
    w[0] = m*(xx-1) + 1 ;
    w[1] = m*xy     + zs;
    w[2] = m*zx     - ys;

    w[4] = m*xy     - zs;
    w[5] = m*(yy-1) + 1 ;
    w[6] = m*yz     + xs;

    w[8] = m*zx     + ys;
    w[9] = m*yz     - xs;
    w[10]= m*(zz-1) + 1 ;
#endif
    matrixMultiplyRotation( u, v, w );
}


// ========================================================================
void
matrixRotateY( const float angleDegrees )
{
    const int    i = gnMatrixView        ; // depth
    /* */ float *u =&gaMatrixView[i+0][0]; // src
    /* */ float *v =&gaMatrixView[i+1][0]; // dst
    /* */ float *w =&gaMatrixView[i+2][0]; // tmp
    matrixCopy( v, u );

    const float  r = DEG_TO_RAD( angleDegrees );
    const float  c = cosf( r );
    const float  s = sinf( r );

    w[ 0] =  c;
    w[ 1] =  0;
    w[ 2] = -s;

    w[ 4] =  0;
    w[ 5] =  1;
    w[ 6] =  0;

    w[ 8] =  s;
    w[ 9] =  0;
    w[10] =  c;

    matrixMultiplyRotation( u, v, w );
}


// ========================================================================
void
matrixStartup()
{
    matrixCopy( &gaMatrixView[ 0 ][0], &MAT4_IDENTITY[0] );
    matrixCopy( &gaMatrixProj[ 0 ][0], &MAT4_IDENTITY[0] );
    matrixCopy( &gaMatrixText[ 0 ][0], &MAT4_IDENTITY[0] );

    gnMatrixView = 0;
    gnMatrixProj = 0;
    gnMatrixText = 0;
}


// Modifies matrix in place
// matrixPush();
//     matrixTranslate( tx, ty, tz );
// matrixPop();
// ========================================================================
void
matrixTranslate( const float tx, const float ty, const float tz )
{
    const int   z = gnMatrixView         ; // depth
    const float*s =&gaMatrixView[ z ][ 0]; // src
    /* */ float*t =&gaMatrixView[ z ][12]; // dst
    const float a = s[ 0];
    const float b = s[ 1];
    const float c = s[ 2];
    const float d = s[ 3];
    const float e = s[ 4];
    const float f = s[ 5];
    const float g = s[ 6];
    const float h = s[ 7];
    const float i = s[ 8];
    const float j = s[ 9];
    const float k = s[10];
    const float l = s[11];

    *t++ += tx*a + ty*e + tz*i; // dot4( <x,y,z,1>, col<0> )
    *t++ += tx*b + ty*f + tz*j; // dot4( <x,y,z,1>, col<1> )
    *t++ += tx*c + ty*g + tz*k; // dot4( <x,y,z,1>, col<2> )
    *t   += tx*d + ty*h + tz*l; // dot4( <x,y,z,1>, col<3> )
}


// postMultiply: v'=M*v
// Where:
//    v = < x,y,z,1> (column vector)
// Examples:
//   v' = P*V*M*v  P=projection V=view M=model
//   v' = S*R*T*v  S=scale R=rotation T=translation
// ========================================================================
float
matrixTransformPoint( float dst[3], const float src[3] )
{
    const int   i = gnMatrixView         ; // depth
    const float*s =&gaMatrixView[ i ][ 0]; // src
    /* */ float w;
    const float x = src[0];
    const float y = src[1];
    const float z = src[2];

    // dot(
    // dst[i] = dot( row(i), col(<x,y,z,1>)) )
    dst[0] = s[0]*x + s[4]*y + s[ 8]*z + s[12];
    dst[1] = s[1]*x + s[5]*y + s[ 9]*z + s[13];
    dst[2] = s[2]*x + s[6]*y + s[10]*z + s[14];
    w      = s[3]*x + s[7]*y + s[11]*z + s[15];

    return w;
}


// postMultiply: v'=M*v
// Where:
//    v = < x,y,z,0> (column vector)
// ========================================================================
void
matrixTransformNormal( float dst[3], const float src[3] )
{
    const int   i = gnMatrixView         ; // depth
    const float*s =&gaMatrixView[ i ][ 0]; // src

    const float x = src[0];
    const float y = src[1];
    const float z = src[2];

    // dot(
    // dst[i] = dot( row(i), col(<x,y,z,1>)) )
    dst[0] = s[0]*x + s[4]*y + s[ 8]*z;
    dst[1] = s[1]*x + s[5]*y + s[ 9]*z;
    dst[2] = s[2]*x + s[6]*y + s[10]*z;
}
