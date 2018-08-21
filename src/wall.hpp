#ifndef WALL_H_
#define WALL_H_

#include "./plane.hpp"
#include "./color.hpp"

struct Wall {
    plane p;
    color c;
};

inline std::ostream& operator<<(std::ostream& os, const Wall&wall) {
    return os << "Wall{ p" << wall.p << " c" << wall.c << "}";
}

#endif  // WALL_H_
