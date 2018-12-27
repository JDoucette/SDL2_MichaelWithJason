// TODO: GCC: ALIGN_GCC(8) ALIGN_MSVC(8) // __attribute__ ((aligned (16)))
    struct Vector2i
    {
        int32_t x, y;

#if 0
        There is no ctor() so one can do:
        Vector2i( const int_t _x, const int_t _y )
        {
            x = _x;
            y = _y;
        }
#endif

inline
friend bool
        operator == ( const Vector2i & lhs, const Vector2i & rhs )
        {
            bool bEqual =
               (lhs.x == rhs.x)
            && (lhs.y == rhs.y);
            return bEqual;
        }

        // TODO: Operator+
    };

