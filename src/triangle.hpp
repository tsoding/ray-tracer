#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

#include "vec.hpp"
#include "wall.hpp"

// TODO(#65): support for triangle meshes from obj files

struct Triangle {
    v3f v1, v2, v3;
};

plane plane_of_triangle(const Triangle &triangle);

bool ray_hits_triangle(const Triangle &triangle,
                       const v3f &ray);

#endif  // TRIANGLE_HPP_
