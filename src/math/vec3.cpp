#include "vec3.h"

#include "utility/strutil.h"

#include <cassert>
#include <cmath>

float Vec3::x() const { return e[0]; }
float Vec3::y() const { return e[1]; }
float Vec3::z() const { return e[2]; }
float Vec3::r() const { return x(); }
float Vec3::g() const { return y(); }
float Vec3::b() const { return z(); }

Vec3 const& Vec3::operator+() const { return *this; }
Vec3 Vec3::operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
float Vec3::operator[](size_t i) const { return e[i]; }
float& Vec3::operator[](size_t i) { return e[i]; }

Vec3& Vec3::operator+=(Vec3 const& o)
{
    e[0] += o.e[0];
    e[1] += o.e[1];
    e[2] += o.e[2];
    return *this;
}

Vec3& Vec3::operator-=(Vec3 const& o)
{
    e[0] -= o.e[0];
    e[1] -= o.e[1];
    e[2] -= o.e[2];
    return *this;
}

Vec3& Vec3::operator*=(Vec3 const& o)
{
    e[0] *= o.e[0];
    e[1] *= o.e[1];
    e[2] *= o.e[2];
    return *this;
}
Vec3& Vec3::operator/=(Vec3 const& o)
{
    e[0] /= o.e[0];
    e[1] /= o.e[1];
    e[2] /= o.e[2];
    return *this;
}

Vec3& Vec3::operator*=(float const c)
{
    e[0] *= c;
    e[1] *= c;
    e[2] *= c;
    return *this;
}
Vec3& Vec3::operator/=(float const c)
{
    e[0] /= c;
    e[1] /= c;
    e[2] /= c;
    return *this;
}

float Vec3::length() const
{
    return sqrt(squared_length());
}

float Vec3::squared_length() const
{
    return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
}

void Vec3::make_unit_vector()
{
    float k = 1.0f / squared_length();
    *this *= k;
}

bool Vec3::is_nan() const
{
    return std::isnan(e[0]) || std::isnan(e[1]) || std::isnan(e[2]);
}

std::string Vec3::to_string() const
{
    return Str_Util::format("%f, %f, %f", e[0], e[1], e[2]);
}

std::istream& operator>>(std::istream& is, Vec3& v)
{
    return is >> v.e[0] >> v.e[1] >> v.e[2];
}

std::ostream& operator<<(std::ostream& os, Vec3& v)
{
    return os << v.e[0] << " " << v.e[1] << " " << v.e[2];
}

bool operator==(Vec3 const& lhs, Vec3 const& rhs)
{
    return (lhs.e[0] == rhs.e[0]) && (lhs.e[1] == rhs.e[1]) &&
        (lhs.e[2] == rhs.e[2]);
}

bool operator!=(Vec3 const& lhs, Vec3 const& rhs)
{
    return !(lhs == rhs);
}

Vec3 operator+(Vec3 const& v1, Vec3 const& v2)
{
    Vec3 u(v1);
    u += v2;
    return u;
}
Vec3 operator-(Vec3 const& v1, Vec3 const& v2)
{
    Vec3 u(v1);
    u -= v2;
    return u;
}

Vec3 operator*(Vec3 const& v1, Vec3 const& v2)
{
    Vec3 u(v1);
    u *= v2;
    return u;
}

Vec3 operator/(Vec3 const& v1, Vec3 const& v2)
{
    Vec3 u(v1);
    u /= v2;
    return u;
}

Vec3 operator*(Vec3 const& v, float const c)
{
    Vec3 u(v);
    u *= c;
    return u;
}

Vec3 operator*(float const c, Vec3 const& v)
{
    return v * c;
}

Vec3 operator/(Vec3 const& v, float const c)
{
    Vec3 u(v);
    u /= c;
    return u;
}

float dot(Vec3 const& v1, Vec3 const& v2)
{
    Vec3 u = v1 * v2;
    return u.e[0] + u.e[1] + u.e[2];
}
Vec3 cross(Vec3 const& v1, Vec3 const& v2)
{
    return Vec3((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
                (-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
                (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}

Vec3 unit_vector(Vec3 const& v)
{
    assert(v.length() > 0);
    return v / v.length();
}
