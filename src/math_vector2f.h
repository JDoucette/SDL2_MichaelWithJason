    struct Vector2f
    {
        float x, y;

/*
        There is no ctor() so one can do:
        Vector3f = { x, y, z };
        Vector3f( int _x, int _y, int _z )
        {
            x = (float)_x;
            y = (float)_y;
            z = (float)_z;
        }
*/
        Vector2f& operator += ( const Vector2f & rhs )
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        Vector2f& operator -= ( const Vector2f & rhs )
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        Vector2f& operator *= (const float scale)
        {
            x *= scale;
            y *= scale;
            return *this;
        }

        Vector2f& operator /= (const float scale)
        {
            x /= scale;
            y /= scale;
            return *this;
        }

        // Remove fractional aspect of floats -- grid aligns the float to 0/16 m
inline
        void Quantize()
        {
            const int ix = (int) x;
            const int iy = (int) y;
            x = (float) ix;
            y = (float) iy;
        }

inline
        Vector2i ToVector2i() const
        {
            Vector2i v2i = { (int)x, (int)y };
            return v2i;
        }

inline
        Vector2i ToAbsGrid()
        {
            Vector2i v = { (int)( x * 16.f ), (int)( y * 16.f ) };
            return v;
        }

#if 0
       // TODO: Do We need: FromVector3i() ??
inline
       Vector2f& operator =( const Vector2i v2i )
       {
           x = (float)v3i.x;
           y = (float)v3i.y;
           return *this;
       }
#endif

inline friend
        float
        dot( const Vector2f & lhs, const Vector2f & rhs )
        {
            return (lhs.x * rhs.x) + (lhs.y * rhs.y);
        }

        float lengthSquared() const
        {
            return dot( *this, *this );
        }

        // @return: sqrt( lengthSuared )
        float length() const
        {
            //float length = lengthSquared();
            //if( length > 0.f ) return sqrtf( length );
            //else               return 0.f;
            const float len2 = lengthSquared();
            return len2 * invsqrtf( len2 );
        }

inline
friend bool
        operator == ( const Vector2f & lhs, const Vector2f & rhs )
        {
            bool bEqual =
               (lhs.x == rhs.x)
            && (lhs.y == rhs.y);
            return bEqual;
        }

inline
friend bool
        operator != ( const Vector2f & lhs, const Vector2f & rhs )
        {
            bool bNotEqual =
               (lhs.x != rhs.x)
            || (lhs.y != rhs.y);
            return bNotEqual;
        }
    };

