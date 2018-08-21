#ifndef SPHERE_HPP_
#define SPHERE_HPP_

#include "vec.hpp"

struct Sphere {
    vec3<float> center;
    float radius;
};

std::ostream& operator<<(std::ostream& os, const Sphere&sphere);
bool is_ray_inside_of_sphere(const Sphere &sphere, const vec3<float> &ray);

#endif  // SPHERE_HPP_
