#pragma once

#include "strutil.h"

#include <array>
#include <cassert>
#include <iostream>
#include <cmath>
#include <string>

class Vec3
{
public:
    Vec3() : e{} {};
    Vec3(float e0, float e1, float e2) : e{e0, e1, e2} {}

    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float r() const { return x(); }
    inline float g() const { return y(); }
    inline float b() const { return z(); }

    inline const Vec3& operator+() const { return *this; }
    inline Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
    inline float operator[](size_t i) const { return e[i]; }
    inline float& operator[](size_t i) { return e[i]; }

    inline Vec3& operator+=(const Vec3& o);
    inline Vec3& operator-=(const Vec3& o);
    inline Vec3& operator*=(const Vec3& o);
    inline Vec3& operator/=(const Vec3& o);
    inline Vec3& operator*=(const float c);
    inline Vec3& operator/=(const float c);

    inline float length() const;
    inline float squared_length() const;
    inline void make_unit_vector();
    inline bool is_nan() const;

    inline std::string to_string() const;

    std::array<float, 3> e;
};

inline std::istream& operator>>(std::istream& is, Vec3& v);
inline std::ostream& operator<<(std::ostream& os, Vec3& v);

inline bool operator==(const Vec3& lhs, const Vec3& rhs);
inline bool operator!=(const Vec3& lhs, const Vec3& rhs);

inline Vec3 operator+(const Vec3& v1, const Vec3& v2);
inline Vec3 operator-(const Vec3& v1, const Vec3& v2);
inline Vec3 operator*(const Vec3& v1, const Vec3& v2);
inline Vec3 operator/(const Vec3& v1, const Vec3& v2);

inline Vec3 operator*(const Vec3& v, const float c);
inline Vec3 operator*(const float c, const Vec3& v);
inline Vec3 operator/(const Vec3& v, const float c);

inline float dot(const Vec3& v1, const Vec3& v2);
inline Vec3 cross(const Vec3& v1, const Vec3& v2);
inline Vec3 unit_vector(const Vec3& v);

///////////////////////////////////////////////////////////////////////////////

inline Vec3& Vec3::operator+=(const Vec3& o)
{
    e[0] += o.e[0];
    e[1] += o.e[1];
    e[2] += o.e[2];
    return *this;
}

inline Vec3& Vec3::operator-=(const Vec3& o)
{
    e[0] -= o.e[0];
    e[1] -= o.e[1];
    e[2] -= o.e[2];
    return *this;
}

inline Vec3& Vec3::operator*=(const Vec3& o)
{
    e[0] *= o.e[0];
    e[1] *= o.e[1];
    e[2] *= o.e[2];
    return *this;
}
inline Vec3& Vec3::operator/=(const Vec3& o)
{
    e[0] /= o.e[0];
    e[1] /= o.e[1];
    e[2] /= o.e[2];
    return *this;
}

inline Vec3& Vec3::operator*=(const float c)
{
    e[0] *= c;
    e[1] *= c;
    e[2] *= c;
    return *this;
}
inline Vec3& Vec3::operator/=(const float c)
{
    e[0] /= c;
    e[1] /= c;
    e[2] /= c;
    return *this;
}

inline float Vec3::length() const
{
    return sqrt(squared_length());
}

inline float Vec3::squared_length() const
{
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

inline void Vec3::make_unit_vector()
{
    float k = 1.0f / squared_length();
    *this *= k;
}

inline bool Vec3::is_nan() const
{
    return std::isnan(e[0]) || std::isnan(e[1]) || std::isnan(e[2]);
}

inline std::string Vec3::to_string() const
{
    return Str_Util::format("%f, %f, %f", e[0], e[1], e[2]);
}

inline std::istream& operator>>(std::istream& is, Vec3& v)
{
    return is >> v.e[0] >> v.e[1] >> v.e[2];
}

inline std::ostream& operator<<(std::ostream& os, Vec3& v)
{
    return os << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

inline bool operator==(const Vec3& lhs, const Vec3& rhs)
{
    return (lhs.e[0] == rhs.e[0]) && (lhs.e[1] == rhs.e[1]) &&
           (lhs.e[2] == rhs.e[2]);
}

inline bool operator!=(const Vec3& lhs, const Vec3& rhs)
{
    return !(lhs == rhs);
}

inline Vec3 operator+(const Vec3& v1, const Vec3& v2)
{
    Vec3 u(v1);
    u += v2;
    return u;
}
inline Vec3 operator-(const Vec3& v1, const Vec3& v2)
{
    Vec3 u(v1);
    u -= v2;
    return u;
}

inline Vec3 operator*(const Vec3& v1, const Vec3& v2)
{
    Vec3 u(v1);
    u *= v2;
    return u;
}

inline Vec3 operator/(const Vec3& v1, const Vec3& v2)
{
    Vec3 u(v1);
    u /= v2;
    return u;
}

inline Vec3 operator*(const Vec3& v, const float c)
{
    Vec3 u(v);
    u *= c;
    return u;
}

inline Vec3 operator*(const float c, const Vec3& v)
{
    return v * c;
}

inline Vec3 operator/(const Vec3& v, const float c)
{
    Vec3 u(v);
    u /= c;
    return u;
}

inline float dot(const Vec3& v1, const Vec3& v2)
{
    Vec3 u = v1 * v2;
    return u.e[0] + u.e[1] + u.e[2];
}
inline Vec3 cross(const Vec3& v1, const Vec3& v2)
{
    return Vec3((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
                (-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
                (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}

inline Vec3 unit_vector(const Vec3& v)
{
    assert(v.length() > 0);
    return v / v.length();
}
