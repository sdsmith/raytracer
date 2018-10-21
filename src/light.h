#pragma once

#include "math/vec3.h"

Vec3 reflect(Vec3 const& v, Vec3 const& n);

bool refract(Vec3 const& v, Vec3 const& n, float ni, float nt, Vec3& refracted);

float schlick(float cosine, float refraction_index);
