#ifndef WALL_H_
#define WALL_H_

#include "./color.hpp"

using plane = vec<float, 4>;

struct Wall {
    plane p;
    color c;
};

std::ostream& operator<<(std::ostream& os, const Wall&wall);
bool is_ray_behind_wall(const Wall &wall, const vec3<float> &ray);

#endif  // WALL_H_
