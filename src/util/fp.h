#pragma once

#include <math.h>
#include <float.h>

float fast_invsqrt( float number )
{
    // https://en.wikipedia.org/wiki/Fast_inverse_square_root
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck? 
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

inline float    fast_sqrt( float x )                    { return x * fast_invsqrt( x ); }

inline float    lerp( float a, float b, float alpha )   { return a + alpha * ( b - a ); }
inline float    clamp( float x, float min, float max )  { return fminf( max, fmaxf( x, min ) ); }

inline bool     fp_equals( float a, float b )           { return fabsf( a - b ) < FLT_EPSILON; }

inline float    to_degrees( float r )                   { return r * ( 180.0f / M_PI ); }
inline float    to_radians( float d )                   { return d * ( M_PI / 180.0f ); }


inline float lerp_angle( float a, float b, float alpha )
{
    a = fmodf( a, M_2_PI );
    b = fmodf( b, M_2_PI );
    
    if ( a > b ) a -= M_2_PI;
    
    return lerp( a, b, alpha );
}

