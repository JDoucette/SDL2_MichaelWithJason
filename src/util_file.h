// Prototype
    size_t File_Size( const char *pathname );

// ======================================================================== 
void
File_Close( FILE *file )
{
    if( file )
        fclose( file );
}


// To test if a directory exists, leave off the trailing forward slash
// ======================================================================== 
bool
File_Exists( const char *pathname, bool *isDirectory_ )
{
    struct stat info;

    int result = stat( pathname, &info );
    if( result != 0 )
        return false;

    if(  isDirectory_ )
        *isDirectory_ = (info.st_mode & S_IFDIR) ? true : false;

    return true;
}


// ======================================================================== 
FILE*
File_OpenGetSize( const char *pathname, size_t *filesize_ )
{
    if( filesize_ )
       *filesize_ = File_Size( pathname );

    FILE *pFile = fopen( pathname, "rb" );
    if( pFile )
        return pFile;

    return NULL;
}


// ======================================================================== 
FILE*
File_OpenWrite( const char *pathname )
{
    FILE *pFile = fopen( pathname, "wb" );
    if( pFile )
        return pFile;

    return NULL;
}


// ======================================================================== 
bool
File_Read( FILE* file, void *buffer, const size_t bytes ) // TODO: FIXME: Shouldn't the order be: size, buffer?
{
    bool valid = false;

    if( file && buffer ) // TODO: #if DEBUG if( file && buffer ) #else if( file )
    {
        const size_t read = fread( buffer, bytes, 1, file );
        valid = (read != 0);  // TODO: FIXME: may have EOF
    }

    return valid;
}


/** Will return 0 if file doesn't exist
    @return size_t - File Size in bytes
*/
// ======================================================================== 
size_t
File_Size( const char *pathname )
{
    struct stat info;

    // A common idiom is fseek( file, 0, SEEK_END ); ftell(); fseek( file, 0, SEEK_SET );
    // But that has 2 problems
    // a) limited to 2 GB filesize
    // b) is insecure
    // instead we use the portable stat()
    // Note: fstat() is not avaialble under Windows
    // https://www.securecoding.cert.org/confluence/display/c/FIO19-C.+Do+not+use+fseek%28%29+and+ftell%28%29+to+compute+the+size+of+a+regular+file
    int result = stat( pathname, &info );
    if( result != 0 )
        return 0; // doesn't exist
    else
        return (size_t) info.st_size;
}


// ======================================================================== 
bool
File_Write( FILE* file, void *buffer, const size_t bytes )
{
    bool valid = false;

    if( file )
    {
        const size_t wrote = fwrite( buffer, 1, bytes, file );
        valid = (wrote == bytes );
    }

    return valid;
}
