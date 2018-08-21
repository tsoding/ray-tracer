#include "./sphere.hpp"

std::ostream& operator<<(std::ostream& os, const Sphere&sphere) {
    return os << "Sphere{ center "
              << sphere.center
              << " radius "
              << sphere.radius
              << "}";
}

bool is_ray_inside_of_sphere(const Sphere &sphere, const vec3<float> &ray) {
    return sqr_norm(sphere.center - ray) < sphere.radius * sphere.radius;
}
