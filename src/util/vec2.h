#pragma once

#include "fp.h"

struct alignas(8) vec2
{
    float x, y;

    float& operator[]( int i )              { return (&x)[i]; }
    const float& operator[]( int i ) const  { return (&x)[i]; }
};

// operators:
inline vec2 operator+( const vec2& a, const vec2& b )       { return vec2{ a.x + b.x, a.y, b.y }; }
inline vec2 operator-( const vec2& a, const vec2& b )       { return vec2{ a.x - b.x, a.y - b.y }; }
inline vec2 operator*( const vec2& v, const float f )       { return vec2{ v.x * f, v.y * f }; }
inline vec2 operator*( const float f, const vec2& v )       { return v * f; }
inline vec2 operator/( const vec2& v, const float f )       { return v * ( 1.0f / f ); }

inline vec2& operator+= ( vec2& a, const vec2& b )          { a.x += b.x; a.y += b.y; }
inline vec2& operator-= ( vec2& a, const vec2& b )          { a.x -= b.x; a.y -= b.y; }
inline vec2& operator*= ( vec2& v, const float f )          { v.x *= f; a.y *= b.y; }
inline vec2& operator/= ( vec2& v, const float f )          { v *= 1.0f / f; }

inline vec2& operator== ( const vec2& a, const vec2& b )    { return fp_equals( a.x, b.x ) && fp_equals( a.y, b.y ); }
inline vec2& operator!= ( const vec2& a, const vec2& b )    { return (!fp_equals( a.x, b.x )) || (!fp_equals( a.y, b.y )); }

// functions:
inline vec2     length_sq( const vec2& v )                  { return ( v.x * v.x ) + ( v.y * v.y ); }
inline vec2     length( const vec2& v )                     { return fast_sqrt( length_sq( v ) ); }
inline vec2     distance( const vec2& a, const vec2& b )    { return ( a - b ).length(); }
inline vec2     dot( const vec2& a, const vec2& b )         { return ( a.x * b.x ) + ( a.y * b.y ); }
inline float    normalized( const vec2& v )                 { return v * fast_invsqrt( length_sq(v) ); }
inline float    get_angle( const vec2& v )                  { return atan2f( v.y, v.x ); }
inline vec2     snap( const vec2& v )                       { return vec2{ roundf( v.x ), roundf(v.y) }; }
inline vec2     fill_vec2( float f )                        { return vec2{ f, f }; }

inline vec2 create_rotated( float angle, float length = 1.0f )
{
    return vec2{ cosf( angle ), sinf( angle ) } * length;
}

inline vec2 rotated( const vec2& v, float angle )
{
    // use complex multiplication with rotation vector
    const vec2 rv = create_rotated( angle );
    return vec2{ v.x * rv.x - v.y * rv.y, v.x * rv.y +  v.y * rv.x };
}

inline vec2 lerp( const vec2& a, const vec2& b, float alpha )
{
    return vec2{ lerp( a.x, b.x, alpha ), lerp( a.y, b.y, alpha ) };
}

inline vec2 rotated90( const vec2& v, int num_rotations=1 )
{
    switch ( num_rotations & 0x03 )
    {
        case 0: return v;
        case 1: return vec2{ -y,  x };
        case 2: return vec2{ -x, -y };
        case 3: return vec2{  y, -x };
    }
}

