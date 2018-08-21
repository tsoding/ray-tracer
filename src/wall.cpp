#include "./wall.hpp"

std::ostream& operator<<(std::ostream& os, const Wall&wall) {
    return os << "Wall{ p" << wall.p << " c" << wall.c << "}";
}

static float dot(const plane &p, const vec3<float> &v) {
    return p.v[0] * v.v[0] + p.v[1] * v.v[1] + p.v[2] * v.v[2] + p.v[3];
}

bool is_ray_behind_wall(const Wall &wall, vec3<float> ray) {
    return dot(wall.p, ray) < 0.0f;
}
