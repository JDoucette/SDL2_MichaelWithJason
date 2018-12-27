// Rotate and Translate is hard-coded to modelview, perspective hard-coded to projection

// = Includes =
//    #include "opengl_matrix.h"

// = Types =
    enum MatrixStack_t
    {
          MATRIX_VIEW
        , MATRIX_PROJECTION
        , MATRIX_TEXTURE
        , NUM_MATRIX_STACKS
    };

// = Constants =
    const int MATRIXSTACK_MAX_DEPTH = 32;

    extern const float MAT4_IDENTITY[16];

// = Globals =

    //  Value
    extern float gaMatrixView[ MATRIXSTACK_MAX_DEPTH ][16];
    extern float gaMatrixProj[ 4                     ][16];
    extern float gaMatrixText[ 4                     ][16];

    // Depth
    extern int  gnMatrixView;
    extern int  gnMatrixProj;
    extern int  gnMatrixText;

// = Prototypes =
// :sort /.*\%18v/

        // This is a fast hard-coded matrix stack
        //  LoadIdentity() // set view matrix
        //  PeekProj()     // returns the projection matrix
        //  PeekView()     // returns the modelview  matrix

inline  /* */ void   matrixCopy( float *pDst, const float *pSrc ) { memcpy( pDst, pSrc, sizeof( float ) * 16 ); }
inline  /* */ void   matrixLoadProj( const float * mProj ) { matrixCopy( &gaMatrixProj[ gnMatrixProj ][0], mProj ); }
inline  /* */ void   matrixLoadView( const float * mView ) { matrixCopy( &gaMatrixView[ gnMatrixView ][0], mView ); }
inline  /* */ void   matrixLoadIdentity()                         { memcpy( &gaMatrixView[ gnMatrixView ][0], &MAT4_IDENTITY[0], sizeof( float ) * 16 ); }
        /* */ void   matrixMultiply( float dst[16], const float lhs[16], const float rhs[16] );
        /* */ void   matrixMultiplyRotation( float dst[16], const float lhs[16], const float rhs[16] );
        /* */ void   matrixOrtho( const int screenWidth, const int screenHeight );
inline  const float* matrixPeekProj() { return &gaMatrixProj[ gnMatrixProj ][0]; }
inline  const float* matrixPeekView() { return &gaMatrixView[ gnMatrixView ][0]; }
        /* */ void   matrixPerspectiveX( const float fovXdegrees, const float aspectRatio, const float zNear, const float zFar );
        /* */ void   matrixPerspectiveY( const float fovXdegrees, const float aspectRatio, const float zNear, const float zFar );
        /* */ void   matrixPop();
        /* */ void   matrixPrint( const float *m, const char *pHeaderText = 0 );
        /* */ void   matrixPush();
        /* */ void   matrixPushTranslateXY( const float x, const float y );
        /* */ void   matrixRotate ( const float angleDegrees, float axisX, float axisY, float axisZ );
        /* */ void   matrixRotateY( const float angleDegrees );
        /* */ void   matrixStartup();
        /* */ void   matrixTranslate( const float tx, const float ty, const float tz );
        /* */ float  matrixTransformPoint ( float dst[3], const float src[3] );
        /* */ void   matrixTransformNormal( float dst[3], const float src[3] );

