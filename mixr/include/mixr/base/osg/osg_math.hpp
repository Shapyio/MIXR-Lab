/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2004 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

#ifndef __mixr_base_osg_math_H__
#define __mixr_base_osg_math_H__

#include <cmath>

namespace mixr {
namespace base {
namespace osg {

// define the standard trig values
#ifdef PI
#undef PI
#undef PI_2
#undef PI_4
#endif
const double PI   = 3.14159265358979323846;
const double PI_2 = 1.57079632679489661923;
const double PI_4 = 0.78539816339744830962;
const double LN_2 = 0.69314718055994530942;
const double INVLN_2 = 1.0 / LN_2;

//
// return the minimum of two values, equivalent to std::min.
// std::min not used because of STL implementation under IRIX not
// containing std::min.
//
template<typename T>
inline T absolute(T v)   { return v < static_cast<T>(0) ? -v : v; }

//
// return true if float lhs and rhs are equivalent,
// meaning that the difference between them is less than an epsilon value
// which defaults to 1e-6.
//
inline bool equivalent(float lhs, float rhs, float epsilon=1e-6)
       { float delta = rhs - lhs; return delta < 0.0f ? delta >= -epsilon : delta <= epsilon; }

//
// return true if double lhs and rhs are equivalent,
// meaning that the difference between them is less than an epsilon value
// which defaults to 1e-6.
//
inline bool equivalent(double lhs,double rhs, double epsilon=1e-6)
                                              { double delta = rhs - lhs; return delta < 0.0 ? delta >= -epsilon : delta <= epsilon; }

//
// return the minimum of two values, equivalent to std::min.
// std::min not used because of STL implementation under IRIX not containing
// std::min.
//
template<typename T>
inline T minimum(T lhs,T rhs)                 { return lhs < rhs ? lhs : rhs; }

//
// return the maximum of two values, equivalent to std::max.
// std::max not used because of STL implementation under IRIX not containing
// std::max.
//
template<typename T>
inline T maximum(T lhs, T rhs)                { return lhs > rhs ? lhs : rhs; }

template<typename T>
inline T clampTo(T v, T minimum,T maximum)    { return v < minimum ? minimum : v > maximum ? maximum : v; }

template<typename T>
inline T clampAbove(T v, T minimum)           { return v < minimum ? minimum : v; }

template<typename T>
inline T clampBelow(T v, T maximum)           { return v > maximum ? maximum : v; }

template<typename T>
inline T clampBetween(T v, T minimum, T maximum)
                                              { return clampBelow(clampAbove(v, minimum), maximum); }

template<typename T>
inline T sign(T v)                            { return v < static_cast<T>(0) ? static_cast<T>(-1) : static_cast<T>(1); }

template<typename T>
inline T signOrZero(T v)                      { return v < static_cast<T>(0) ? static_cast<T>(-1) : ( v > static_cast<T>(0) ? static_cast<T>(1) : 0 ); }

template<typename T>
inline T square(T v)                          { return v * v; }

template<typename T>
inline T signedSquare(T v)                    { return v < static_cast<T>(0) ? -v * v : v * v; }

inline float inDegrees(float angle)           { return angle * static_cast<float>(PI) / 180.0f; }
inline double inDegrees(double angle)         { return angle * PI / 180.0; }

template<typename T>
inline T inRadians(T angle)                   { return angle; }

inline float DegreesToRadians(float angle)    { return angle * static_cast<float>(PI)/180.0f; }
inline double DegreesToRadians(double angle)  { return angle * PI / 180.0; }

inline float RadiansToDegrees(float angle)    { return angle * 180.0f / static_cast<float>(PI); }
inline double RadiansToDegrees(double angle)  { return angle * 180.0 / PI; }

inline float round(float v)                   { return v>=0.0f ? std::floor(v+0.5f) : std::ceil(v-0.5f); }
inline double round(double v)                 { return v>=0.0  ? std::floor(v+0.5)  : std::ceil(v-0.5); }

inline bool isNaN(double v)                   { return std::isnan(v); }

// compute the volume of a tetrahedron
template<typename T>
inline float computeVolume(const T& a,const T& b,const T& c,const T& d)
{
    return std::fabs(((b-c)^(a-b))*(d-b));
}

// compute the volume of a prism
template<typename T>
inline float computeVolume(const T& f1,const T& f2,const T& f3,
                           const T& b1,const T& b2,const T& b3)
{
    return computeVolume(f1,f2,f3,b1)+
           computeVolume(b1,b2,b3,f2)+
           computeVolume(b1,b3,f2,f3);
}

// Convert a ascii number to a double, ignoring locale settings
extern double asciiToDouble(const char* str);

// Convert a ascii number to a float, ignoring locale settings
inline float asciiToFloat(const char* str)        { return static_cast<float>(asciiToDouble(str)); }

// Detect first ascii POSITIVE number in string and convert to double
extern double findAsciiToDouble(const char* str);

// Detect first ascii POSITIVE number in string and convert to double
inline float findAsciiToFloat(const char* str)    { return static_cast<float>(findAsciiToDouble(str)); }

}
}
}

#endif

