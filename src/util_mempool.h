#if DEBUG
    #define _DEBUG_MEM_POOL   0 // normally 1
    #define _PROFILE_MEM_POOL 1 // normally 0
#elif PROFILE
    #define _DEBUG_MEM_POOL   0
    #define _PROFILE_MEM_POOL 1
#else // RELEASE
    #define _DEBUG_MEM_POOL   0
    #define _PROFILE_MEM_POOL 0
#endif

#define FAKE_MEM_POOL 0

#if FAKE_MEM_POOL
    bool gbFakeMemPoolDirty = false;
#endif

    #undef  _SYSTEM_
    #define _SYSTEM_ "MEMPOOL"

// TODO: Trace & Profile message levels
// Level 0 n/a
// Level 1 profile
// Level 2 debug only
// Level 3 profile + debug
#if _DEBUG_MEM_POOL | _PROFILE_MEM_POOL
    #define   TRACE_MEM_POOL(text) if(  _DEBUG_MEM_POOL) {TRACE("MemPool: ");TRACE(text);}
    #define PROFILE_MEM_POOL(text) if(_PROFILE_MEM_POOL) {TRACE("MemPool: ");TRACE(text);}
#else
    #define   TRACE_MEM_POOL(text)
    #define PROFILE_MEM_POOL(text)
#endif

    const int      _1K = 1024;
    const size_t  _64K =   64 * _1K;
    const size_t _128K =  128 * _1K;
    const size_t   _1M = 1024 * _1K;
    const size_t   _8M =    8 * _1M;

    /** These are the number of alive pools.  The other dead pools are simply place holders.
        The pool index, #, determines the entry size or block sizein bytes: EntrySize = 2^#. 
        Each pool may have a different capacity - the maximum number of blocks.

        +-------------+-------------+-----+--------------+
        | :           | :           | ... | :            |
        | Pool[ 8][0] | Pool[ 8][1] | ... | Pool[ 8][31] | EntrySize = 256 bytes, Capacity = 32
        | :           | :           | ... | :            |
        | Pool[16][0] | Pool[16][1] |     | Pool[16][7]  | EntrySize = 64K, Capacity = 2
        | :           | :           | ... | :            |
        | Pool[23][0] | Pool[23][1] | ... | Pool[23][15] | EntrySize = 1MB, Capacity = 16
        | :           | :           | ... | :            |
        |NUM_MEM_POOLS|             |     |              |
        +-------------+-------------+-----+--------------+
    */
    enum MemPoolType_e
    {
         MEM_POOL_32   =  5 //  2^5   32
        ,MEM_POOL_64   =  6 //  2^6   64
        ,MEM_POOL_128  =  7 //  2^7  128
        ,MEM_POOL_256  =  8 //  2^8  256
        ,MEM_POOL_512  =  9 //  2^9  512
        ,MEM_POOL_1K   = 10 // 2^10   1K
        ,MEM_POOL_2K   = 11 // 2^11   2K
        ,MEM_POOL_4K   = 12 // 2^12   4K
        ,MEM_POOL_8K   = 13 // 2^13   8K
        ,MEM_POOL_16K  = 14 // 2^14  16K
        ,MEM_POOL_32K  = 15 // 2^15  32K
        ,MEM_POOL_64K  = 16 // 2^16  64K
        ,MEM_POOL_128K = 17 // 2^17 128K
        ,MEM_POOL_256K = 18 // 2^18 256K
        ,MEM_POOL_512K = 19 // 2^19 512K
        ,MEM_POOL_1M   = 20 // 2^20   1M
        ,MEM_POOL_2M   = 21 // 2^21   2M
        ,MEM_POOL_4M   = 22 // 2^22   4M
        ,MEM_POOL_8M   = 23 // 2^23   8M
        ,MEM_POOL_16M  = 24 // 2^24  16M
        ,NUM_MEM_POOLS = 25
    };

// Private

    struct MemPoolDebug_t
    {
#if _PROFILE_MEM_POOL
        /*       */ uint32_t nFrameAcquired; // need to do this per block
        /*       */ uint32_t nFrameReleased;
#endif
    };

// Forward declare
#if _DEBUG_MEM_POOL | _PROFILE_MEM_POOL
    void  MemPool_GetName( MemPoolType_e ePool, char *name_ ); // deprecated
    char* MemPool_GetIdNameSize( int iPool, size_t size );
#endif // DEBUG PROFILE

    struct MemPool_t;
    MemPool_t *_gpMemPool = 0;

// Types
    struct MemPool_t
    {                               // Size
        uint16_t        iActive   ; // 2 active  number of blocks; everything before this is alive, everything after is dead
        uint16_t        nBlocks   ; // 2 maximum number of blocks
        uint32_t        nBlockSize; // 4 bytes per one block
        uint8_t       **pBlocks   ; // 8 array of pointers: -> MemBlock[0 .. nBlocks-1]
        uint8_t        *pData     ; // 8 raw total memory : contiguous array of MemPoolBlock[ nBlocks ]
#if _DEBUG_MEM_POOL | _PROFILE_MEM_POOL | FAKE_MEM_POOL // +8
        uint32_t        nFailed   ; //   Number of "failed" first-fit allocations
        MemPoolDebug_t *pProfile  ; //==
#endif                              //32

        uint32_t GetCapacity() const { return nBlocks; }

        void Startup( MemPoolType_e type, uint16_t capacity )
        {
            /*  */   iActive    = 0;
            /*  */   nBlocks    = capacity;
            /*  */   nBlockSize = (1 << type);
            size_t   nTotalSize = nBlockSize * nBlocks;

            if( nTotalSize < nBlocks ) // detect overflow !
            {
TRACE( "[%2d]: Block: %7u bytes * %u = Total: %s\n", type, nBlockSize, nBlocks, nTotalSize );
                char text[ 256 ];
                sprintf( text,
                    "\n\n"
                    "Trying too allocate too many blocks in memory pool!\n\n"
                    "Pool: %u\n"
                    "Blocks: %u * BlockSize: %u = Total: %u"
                    , type, nBlocks, nBlockSize, (uint32_t) nTotalSize
                );
                Game_Fatal( _SYSTEM_, text );
                nBlocks = 0;
            }

#if _DEBUG_MEM_POOL | _PROFILE_MEM_POOL |FAKE_MEM_POOL
            /*  */   nFailed    = 0;
#endif

            if( nBlocks )
            {
                     pBlocks    = (uint8_t**) malloc( sizeof(uint8_t*) * nBlocks );
                     pData      = (uint8_t*)  malloc( nTotalSize );
#if _PROFILE_MEM_POOL
                     pProfile   = (MemPoolDebug_t*) malloc( sizeof(MemPoolDebug_t) * nBlocks );
#endif
            } else {
                     pBlocks    = NULL;
                     pData      = NULL;
            }
            uint8_t *pSrc       = pData;

            for( uint32_t iBlock = 0; iBlock < nBlocks; iBlock++, pSrc += nBlockSize )
                pBlocks[ iBlock ] = pSrc;
        }

        void Shutdown()
        {
#if DEBUG
#endif
            free( pData   );
            free( pBlocks );
        }
    };

    // TODO: FIXME: These need to be duplicated across every core
    MemPool_t gaMemPool[ NUM_MEM_POOLS ]; // NOTE: Keep in sync with MemPoolType_e

    // Work around warning -Wmissing-field-initializers
    // which wants us to specificy all initializer fields
    // by providing the capacity in a separate initializer
    uint16_t gaMemPoolSizeInit[ NUM_MEM_POOLS ] =
    { // Capacity = Maximum number of entries for this pool #, where EntrySize = (1 << #)
#if FAKE_MEM_POOL // Default zero in order to query/tune actual values
            0 // 2^ 0
        ,   0 // 2^ 1
        ,   0 // 2^ 2
        ,   0 // 2^ 3
        ,   0 // 2^ 4
        ,   0 // 2^ 5
        ,   0 // 2^ 6 MEM_POOL_64
        ,   0 // 2^ 7 MEM_POOL_128
        ,   0 // 2^ 8 MEM_POOL_256
        ,   0 // 2^ 9
        ,   0 // 2^10 MEM_POOL_1K
        ,   0 // 2^11
        ,   0 // 2^12
        ,   0 // 2^13
        ,   0 // 2^14TRACE_MEM_POOL
        ,   0 // 2^15
        ,   0 // 2^16 MEM_POOL_64K
        ,   0 // 2^17 MEM_POOL_128K
        ,   0 // 2^18
        ,   0 // 2^19
        ,   0 // 2^20 MEM_POOL_1M
        ,   0 // 2^21
        ,   0 // 2^22
        ,   0 // 2^23 MEM_POOL_8M
#else
  #if 1 // walls
            0 // 2^ 0 // invalid!
        ,   0 // 2^ 1
        ,   0 // 2^ 2
        ,   0 // 2^ 3
        ,   0 // 2^ 4
        ,2048 // 2^ 5 MEM_POOL_32
        ,1024 // 2^ 6 MEM_POOL_64
        ,   0 // 2^ 7 MEM_POOL_128
        ,1024 // 2^ 8 MEM_POOL_256
        ,16384// 2^ 9
        ,8192 // 2^10 MEM_POOL_1K
        ,2048 // 2^11
        ,4096 // 2^12
        ,1024 // 2^13
        , 512 // 2^14
        , 512 // 2^15
        , 128 // 2^16 MEM_POOL_64K
        , 256 // 2^17 MEM_POOL_128K
        , 256 // 2^18 MEM_POOL_256K
        , 512 // 2^19 MEM_POOL_512K
        , 128 // 2^20 MEM_POOL_1M
        ,  32 // 2^21 MEM_POOL_2M
        ,   8 // 2^22 MEM_POOL_4M
        ,   2 // 2^23 MEM_POOL_8M
        ,   5 // 2^24 MEM_POOL_16M // World: wall, terrain, lights, liquids dynamic textures see: gLevel._solidSprite
  #else // TODO: FIXME: CRASH: OSX // WIP: Optimized mem block partitions
            0 // 2^ 0 // invalid!
        ,   0 // 2^ 1
        ,   0 // 2^ 2
        ,   0 // 2^ 3
        ,   0 // 2^ 4
        ,2048 // 2^ 5 MEM_POOL_32
        ,2048 // 2^ 6 MEM_POOL_64
        ,   0 // 2^ 7 MEM_POOL_128
        ,   0 // 2^ 8 MEM_POOL_256
        ,  16 // 2^ 9             // 16384
        , 512 // 2^10 MEM_POOL_1K // 8192
        ,1024 // 2^11
        ,2048 // 2^12 MEM_POOL_4K
        , 512 // 2^13
        , 256 // 2^14 MEM_POOL_16K
        , 128 // 2^15
        , 128 // 2^16 MEM_POOL_64K
        , 128 // 2^17 MEM_POOL_128K
        , 128 // 2^18 MEM_POOL_256K
        , 512 // 2^19 MEM_POOL_512K
        ,  32 // 2^20 MEM_POOL_1M // CRASH: Can't allocate 8 GB !!
        ,   8 // 2^21 MEM_POOL_2M
        ,   8 // 2^22 MEM_POOL_4M
        ,   1 // 2^23 MEM_POOL_8M
        ,   8 // 2^24 MEM_POOL_16M // World: wall, terrain, lights, liquids dynamic textures see: gLevel._solidSprite
  #endif
#endif
    };


#if _DEBUG_MEM_POOL | _PROFILE_MEM_POOL | FAKE_MEM_POOL
    char sMemPoolName[ 256 ];
#endif


// @return int Handle to MemBlock inside the Pool
// ========================================================================
inline int
MemPoolMgr_GetHandle( MemPoolType_e ePool )
{
    MemPool_t *pPool  = &gaMemPool[ ePool ];
    uint32_t   hBlock = pPool->iActive;

    if (hBlock < pPool->nBlocks)
    {
        pPool->iActive++;
        return hBlock;
    }
    else
    {
        char text[ 256 ];
#if _DEBUG_MEM_POOL || _PROFILE_MEM_POOL
        MemPool_GetIdNameSize( ePool, 0 );
        sprintf( text, "\nOUT OF MEMORY!\n\nNo more handles: %s", sMemPoolName );
#else
        sprintf( text, "\nOUT OF MEMORY!\n\nNo more handles" );
#endif
        Game_Fatal( _SYSTEM_, text );
    }

    return 0;
}


/** Given a pool type, return the block size
    @return size in bytes of one block in the pool
*/
// ========================================================================
inline size_t
MemPoolMgr_GetBlockSize( MemPoolType_e ePool )
{
    // 1ULL to avoid C4334 in MSVC conversion from 32-bit to 64-bit
    return ((size_t)1 << ePool);
}



#if _DEBUG_MEM_POOL | _PROFILE_MEM_POOL | FAKE_MEM_POOL
    /** Fill in the buffer with a short human readable pool size (K, or M)
        If the pool is not valid will display an asterisk behind the size.
    */
    // ========================================================================
    void
    MemPool_GetName( MemPoolType_e ePool, char *name_ )
    {
        uint32_t nSize  = (uint32_t)MemPoolMgr_GetBlockSize(ePool);
        bool     bValid = gaMemPool[ ePool ].nBlocks > 0;
        uint32_t nDiv   = 1;
        char     sUnit  = ' ';

        if( nSize>= _1M )
        {
            nDiv  = _1M;
            sUnit =  'M';
        }
        else
        if( nSize>= _1K )
        {
            nDiv  = _1K;
            sUnit =  'K';
        }

        sprintf( name_, "%3d%c%c (#%4d/%4d)"
            , (nSize / nDiv), sUnit, bValid ? ' ' : '*'
            , gaMemPool[ ePool ].iActive
            , gaMemPool[ ePool ].nBlocks - 1
        );
    }


    // ========================================================================
    char*
    MemPool_GetIdNameSize( int iPool, size_t size )
    {
        char    *name_  = sMemPoolName;
        uint32_t nSize  = (uint32_t)MemPoolMgr_GetBlockSize( (MemPoolType_e) iPool );
        bool     bValid = gaMemPool[ iPool ].nBlocks > 0;
        uint32_t nDiv   = 1;
        char     sUnit  = ' ';

        if( nSize>= _1M )
        {
            nDiv  = _1M;
            sUnit =  'M';
        }
        else
        if( nSize>= _1K )
        {
            nDiv  = _1K;
            sUnit =  'K';
        }

        sprintf( name_, "Pool: [%2d] %3d%c%c (#%3d/%4d) %s %s %s"
            , iPool
            , (nSize / nDiv), sUnit, bValid ? ' ' : '*'
            , gaMemPool[ iPool ].iActive
            , gaMemPool[ iPool ].nBlocks - 1
            , itoa_comma( (uint32_t) size )
            , size <= nSize ? "<=" : "**"
            , itoa_comma( (uint32_t) nSize )
        );

        return name_;
    }
#endif // DEBUG PROFILE

// Return the pool # that the blocksize would fit into, or 0 if none.
// ========================================================================
MemPoolType_e
MemPoolMgr_GetPool( const size_t size )
{
    MemPoolType_e ePool = (MemPoolType_e) log2_floor( (uint32_t) size ); // not ceil() since if size already pow2 will round up & waste mem when could fit
    MemPool_t    *pPool = &gaMemPool[ ePool ];

#if _DEBUG_MEM_POOL
    bool bFoundFirstLargestPool = false;
    bool bTrace = false;
#endif

    for( int iPool = (int)ePool; iPool < NUM_MEM_POOLS; iPool++, pPool++ )
    {
#if _DEBUG_MEM_POOL
        if( bTrace )
        {
            MemPool_GetIdNameSize( iPool, size );
            TRACE( "INFO: %s\n", sMemPoolName );
        }
#endif // DEBUG

        uint32_t nBlocks = pPool->nBlocks;

#if FAKE_MEM_POOL
        nBlocks = -1; // always have "free blocks" in this partition
        gbFakeMemPoolDirty = true;
#endif

        if( pPool && nBlocks && (size <= pPool->nBlockSize) )
        {
#if FAKE_MEM_POOL
            pPool->iActive++;
            if( pPool->nFailed < pPool->iActive )
                pPool->nFailed = pPool->iActive ;
#endif
            if( pPool->iActive < nBlocks )
                return (MemPoolType_e)iPool; // NOTE: Pool may be full!
            else //  pPool->iActive >= pPool->nBlocks
            {
#if _DEBUG_MEM_POOL
                if( ! bFoundFirstLargestPool )
                {
                    bFoundFirstLargestPool = true;
                    pPool->nFailed++;
                }
                MemPool_GetIdNameSize( iPool, size );
                TRACE( "FULL! %s\n", sMemPoolName );
#endif // DEBUG
            }
        }
#if _DEBUG_MEM_POOL
        else
        {
            bTrace = true;
            MemPool_GetIdNameSize( iPool, size );
            TRACE( "----: %s\n", sMemPoolName ); // skipping
        }
#endif // DEBUG
    }

    Game_Fatal( _SYSTEM_, "\nNo available memory pools!" );
    return (MemPoolType_e)0; // Invalid Pool
}


// ========================================================================
bool
MemPoolMgr_IsPoolEmpty( MemPoolType_e ePool )
{
    MemPool_t *pPool  = &gaMemPool[ ePool ];
    bool       bEmpty = pPool
        ? (pPool->iActive == 0)
        : false;
    return bEmpty;
}


// ========================================================================
bool
MemPoolMgr_IsPoolFull( MemPoolType_e ePool )
{
    MemPool_t *pPool = &gaMemPool[ ePool ];
    bool       bFull = pPool
        ? (pPool->iActive >= pPool->nBlocks )
        : false;
    return bFull;
}

/** Acquire raw memory in the specified pool and block offset
    @return pointer to raw memory: pMemData
*/
// ========================================================================
inline uint8_t*
MemPoolMgr_LockHandle( const MemPoolType_e ePool, const int hBlock )
{
    MemPool_t *pPool = &gaMemPool[ ePool ];
    return     pPool->pBlocks[ hBlock ];
}


// TOOD:DEBUG PROFILE only?
// ========================================================================
void
MemPoolMgr_Query( size_t *used_, size_t *free_, size_t *total_ )
{
    size_t nUsed  = 0;
    size_t nTotal = 0;
    size_t nFree  = 0;

    for( int iPool = 0; iPool < NUM_MEM_POOLS; iPool++ )
    {
        MemPool_t *pPool = &gaMemPool[ iPool ];
        size_t     nSize = pPool->nBlockSize;
        if( pPool )
        {
            nUsed  += (pPool->iActive * nSize);
            nTotal += (pPool->nBlocks * nSize);
        }
#if FAKE_MEM_POOL
        if( gbFakeMemPoolDirty ) // prevent spam
        {
            char text[256];
            MemPool_GetIdNameSize( iPool, 0 );
            sprintf( text, "    ,%4u // Allocations: %s", pPool->nFailed, sMemPoolName );
            TRACE( "%s\n", text );
        }
#endif // FAKE_MEM_POOL
    }
    nFree = nTotal - nUsed;

#if FAKE_MEM_POOL
    gbFakeMemPoolDirty = false;
#endif // FAKE_MEM_POOL

    if( used_ ) *used_  = nUsed;
    if( free_ ) *free_  = nFree;
    if( total_) *total_ = nTotal;
}


// ========================================================================
void
MemPoolMgr_QueryStats()
{
#if _DEBUG_MEM_POOL | _PROFILE_MEM_POOL
#if 0
    for( int iPool = 0; iPool < NUM_MEM_POOLS; iPool++ )
    {
        MemPool_t *pPool = &gaMemPool[ iPool ];
        if( pPool )
        {
            MemPool_GetIdNameSize( iPool, 0 );
            TRACE( "%s\n", sMemPoolName );
        }
    }
#endif
#endif
}


// ========================================================================
void
MemPoolMgr_Shutdown()
{
    const int  nPool = NUM_MEM_POOLS;
    MemPool_t *pDst  = &gaMemPool[ 0 ];

    for( int iPool = 0; iPool < nPool; iPool++, pDst++ )
        pDst->Shutdown();
}


// ========================================================================
void
MemPoolMgr_Startup()
{
    const int  nPool = NUM_MEM_POOLS;
    MemPool_t *pDst  = &gaMemPool[ 0 ];

    for( int iPool = 0; iPool < nPool; iPool++, pDst++ )
        pDst->Startup( (MemPoolType_e) iPool, gaMemPoolSizeInit[ iPool ] );
}


// Release
// Swap-with-Last
// ========================================================================
void
MemPoolMgr_Unlock( MemPoolType_e ePool, int hBlock )
{
    MemPool_t *pPool = &gaMemPool[ ePool ]     ;
#if !FAKE_MEM_POOL
    int        iDead = pPool->iActive - 1      ;  // Swap: [ hHandle ] <--> [ iActive-1 ]
    uint8_t   *pTemp = pPool->pBlocks[ hBlock ];
                       pPool->pBlocks[ hBlock ] = pPool->pBlocks[ iDead ];
                                                  pPool->pBlocks[ iDead ] = pTemp;
#if _PROFILE_MEM_POOL
    pPool->pProfile[hBlock].nFrameReleased = (uint32_t)gnFrame;
#endif
#endif // FAKE_MEM_POOL

    pPool->iActive--;
}


// ________________________________________________________________________

    /*
    Packed MemPoolBlock_t uint32_t;
    +---------------+------------------+
    | Mem Pool Type | Mem Block Handle |
    +---------------+------------------+
     8-bit           24-bit
    */
    const int MEM_POOL_MASK   = 0xFF      ; //  8 bits PoolType = PoolSize
    const int MEM_BLOCK_MASK  = 0x00FFFFFF; // 24 bits BlockHandle
    const int MEM_POOL_SHIFT  =         24;
    const int MEM_BLOCK_SHIFT =          0;

    struct MemPoolBlockData_t
    {
public:
        uint8_t    *pData;
        uint32_t    nPoolBlock; // packed MemPoolType_e MemBlockOffset

                    MemPoolBlockData_t( const char *filename = 0, size_t line = 0 )
                    {
                        (void)filename;
                        (void)line;
                        pData = 0; nPoolBlock = 0;
                    }

inline  uint8_t*    make( const size_t size )
                    {
                        MemPoolType_e  eMemPool  = MemPoolMgr_GetPool( size );
#if _DEBUG_MEM_POOL
                        TRACE("mempool.make( %s )  Pool: [%2d]  Block: $%d\n", itoa_comma( (uint32_t) size ), eMemPool, hMemBlock );
#endif
#if FAKE_MEM_POOL
                        int hMemBlock = 0;
                        pData = new uint8_t[ size ];
#else
                        int            hMemBlock = MemPoolMgr_GetHandle ( eMemPool );
                        /*uint8_t* */  pData     = MemPoolMgr_LockHandle( eMemPool, hMemBlock );
#endif // FAKE_MEM_POOL
                        nPoolBlock = 0
                            | (uint32_t)(eMemPool  & MEM_POOL_MASK ) << MEM_POOL_SHIFT
                            | (uint32_t)(hMemBlock & MEM_BLOCK_MASK) << MEM_BLOCK_SHIFT;
                        return pData;
                    }

inline  void        free()
                    {
                        int eMemPool  = (nPoolBlock >> MEM_POOL_SHIFT ) & MEM_POOL_MASK ;
                        int hMemBlock = (nPoolBlock >> MEM_BLOCK_SHIFT) & MEM_BLOCK_MASK;
#if FAKE_MEM_POOL
                        delete [] pData;
#else
                        if( !eMemPool )
                            Game_Fatal( _SYSTEM_, "\nTrying to free memory not in the pool!" );
#endif // FAKE_MEM_POOL
                        MemPoolMgr_Unlock( (MemPoolType_e)eMemPool, hMemBlock );
                    }
    };

    MemPoolBlockData_t gLastMemPoolBlockData( "gLastMemPoolBlockData", __LINE__ ); // Mem_Alloc( size ); Mem_Deloc();

// TODO: FIXME: #if DEBUG
#define Mem_Alloc(...) Mem_Alloc_DEBUG( __FILE__, __LINE__, __VA_ARGS__ )

// ========================================================================
inline uint8_t*
Mem_Alloc_DEBUG( const char *filename, const size_t line, const size_t nSize, MemPoolBlockData_t *pPoolBlockData = &gLastMemPoolBlockData )
{
    (void)filename;
    (void)line;
    return pPoolBlockData->make( nSize );
}


// ========================================================================
inline void
Mem_Deloc( MemPoolBlockData_t *pPoolBlockData = &gLastMemPoolBlockData )
{
    pPoolBlockData->free();
}
