#pragma once

#include <array>
#include <iostream>

struct Vec3
{
public:
    Vec3() : e{} {};
    Vec3(float e0, float e1, float e2) : e{{e0, e1, e2}} {}

    float x() const;
    float y() const;
    float z() const;
    float r() const;
    float g() const;
    float b() const;

    Vec3 const& operator+() const;
    Vec3 operator-() const;
    float operator[](size_t i) const;
    float& operator[](size_t i);

    Vec3& operator+=(Vec3 const& o);
    Vec3& operator-=(Vec3 const& o);
    Vec3& operator*=(Vec3 const& o);
    Vec3& operator/=(Vec3 const& o);
    Vec3& operator*=(float const c);
    Vec3& operator/=(float const c);

    float length() const;
    float squared_length() const;
    void make_unit_vector();
    bool is_nan() const;

    std::string to_string() const;

    std::array<float, 3> e;
};

std::istream& operator>>(std::istream& is, Vec3& v);
std::ostream& operator<<(std::ostream& os, Vec3& v);

bool operator==(Vec3 const& lhs, Vec3 const& rhs);
bool operator!=(Vec3 const& lhs, Vec3 const& rhs);

Vec3 operator+(Vec3 const& v1, Vec3 const& v2);
Vec3 operator-(Vec3 const& v1, Vec3 const& v2);
Vec3 operator*(Vec3 const& v1, Vec3 const& v2);
Vec3 operator/(Vec3 const& v1, Vec3 const& v2);

Vec3 operator*(Vec3 const& v, float const c);
Vec3 operator*(float const c, Vec3 const& v);
Vec3 operator/(Vec3 const& v, float const c);

float dot(Vec3 const& v1, Vec3 const& v2);
Vec3 cross(Vec3 const& v1, Vec3 const& v2);
Vec3 unit_vector(Vec3 const& v);
