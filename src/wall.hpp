#ifndef WALL_H_
#define WALL_H_

#include "./color.hpp"

using plane = vec<float, 4>;

struct Wall {
    plane p;
    color c;
};

inline std::ostream& operator<<(std::ostream& os, const Wall&wall) {
    return os << "Wall{ p" << wall.p << " c" << wall.c << "}";
}

inline float dot(const plane &p, const vec3<float> &v) {
    return p.v[0] * v.v[0] + p.v[1] * v.v[1] + p.v[2] * v.v[2] + p.v[3];
}

#endif  // WALL_H_
