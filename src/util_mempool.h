    #undef  _SYSTEM_
    #define _SYSTEM_ "MEMPOOL"

// TODO: Trace & Profile message levels
// Level 0 n/a
// Level 1 profile
// Level 2 debug only
// Level 3 profile + debug
    #define   TRACE_MEM_POOL(text)
    #define PROFILE_MEM_POOL(text)

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
    };

// Forward declare

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

            if( nBlocks )
            {
                     pBlocks    = (uint8_t**) malloc( sizeof(uint8_t*) * nBlocks );
                     pData      = (uint8_t*)  malloc( nTotalSize );
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
    };




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
        sprintf( text, "\nOUT OF MEMORY!\n\nNo more handles" );
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


        if( pPool && nBlocks && (size <= pPool->nBlockSize) )
        {
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
    }
    nFree = nTotal - nUsed;

    if( used_ ) *used_  = nUsed;
    if( free_ ) *free_  = nFree;
    if( total_) *total_ = nTotal;
}


// ========================================================================
void
MemPoolMgr_QueryStats()
{
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
MemPoolMgr_Unlock( MemPoolType_e ePool )
{
    MemPool_t *pPool = &gaMemPool[ ePool ]     ;
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
                        int            hMemBlock = MemPoolMgr_GetHandle ( eMemPool );
                        /*uint8_t* */  pData     = MemPoolMgr_LockHandle( eMemPool, hMemBlock );
                        nPoolBlock = 0
                            | (uint32_t)(eMemPool  & MEM_POOL_MASK ) << MEM_POOL_SHIFT
                            | (uint32_t)(hMemBlock & MEM_BLOCK_MASK) << MEM_BLOCK_SHIFT;
                        return pData;
                    }

inline  void        free()
                    {
                        int eMemPool  = (nPoolBlock >> MEM_POOL_SHIFT ) & MEM_POOL_MASK ;
                        if( !eMemPool )
                            Game_Fatal( _SYSTEM_, "\nTrying to free memory not in the pool!" );
                        MemPoolMgr_Unlock( (MemPoolType_e)eMemPool );
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
