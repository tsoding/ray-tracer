#ifndef SPHERE_HPP_
#define SPHERE_HPP_

#include "vec.hpp"

struct Sphere {
    vec3<float> center;
    float radius;
};

std::ostream& operator<<(std::ostream& os, const Sphere&sphere);

#endif  // SPHERE_HPP_
