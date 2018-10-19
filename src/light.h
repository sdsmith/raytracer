#pragma once

#include "math/vec3.h"

Vec3 reflect(const Vec3& v, const Vec3& n);

bool refract(const Vec3& v, const Vec3& n, float ni, float nt, Vec3& refracted);

float schlick(float cosine, float refraction_index);
