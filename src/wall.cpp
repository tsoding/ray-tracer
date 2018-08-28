#include "./wall.hpp"

std::ostream& operator<<(std::ostream& os, const Wall&wall) {
    return os << "Wall{ p" << wall.p << " c" << wall.c << "}";
}

bool is_ray_behind_wall(const Wall &wall, const vec3<float> &ray) {
    return dot(wall.p, ray) < 0.0f;
}
