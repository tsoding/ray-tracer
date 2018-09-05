#include "./triangle.hpp"

v3f cross_product(const v3f &v1, const v3f &v2) {
    // i      , j      , k
    // v1.v[0], v1.v[1], v1.v[2]
    // v2.v[0], v2.v[1], v2.v[2]
    //
    ////////////////////////////////
    //
    //  i * v1.v[1], v1.v[2]
    //      v2.v[1], v2.v[2]
    //
    // -j * v1.v[0], v1.v[2]
    //      v2.v[0], v2.v[2]
    //
    //  k * v1.v[0], v1.v[1],
    //      v2.v[0], v2.v[1],
    //
    ////////////////////////////////
    //
    // i = v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1]
    // j = v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2]
    // k = v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0]

    return v3f {
        v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1],
        v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2],
        v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0]
    };
}

plane plane_of_triangle(const Triangle &triangle) {
    const v3f ab = triangle.v2 - triangle.v1;
    const v3f bc = triangle.v3 - triangle.v1;
    const v3f xp = normalize(cross_product(ab, bc));

    // xp.v[0] * v1.v[0] + xp.v[1] * v1.v[1] + xp.v[2] * v1.v[2] + d = 0
    const float d = - xp.v[0] * triangle.v1.v[0] - xp.v[1] * triangle.v1.v[1] - xp.v[2] * triangle.v1.v[2];

    return plane {xp.v[0], xp.v[1], xp.v[2], d};
}

float point_segment_distance(const v3f &p, const v3f &a, const v3f &b) {
    const v3f n = b - a;
    const v3f pa = a - p;
    const v3f c = n * (dot(pa, n) / dot(n, n));
    const v3f d = pa - c;
    return sqrtf(dot(d, d));
}

bool ray_hits_triangle(const Triangle &triangle,
                       const v3f &ray) {
    const auto p = plane_of_triangle(triangle);
    const auto n = normalize(vec_pop(p));

    const auto AB = triangle.v2 - triangle.v1;
    const auto BC = triangle.v3 - triangle.v2;
    const auto CA = triangle.v1 - triangle.v3;

    return dot(n, cross_product(AB, ray - triangle.v1)) > 0
        && dot(n, cross_product(BC, ray - triangle.v2)) > 0
        && dot(n, cross_product(CA, ray - triangle.v3)) > 0;
}

// The point is in the triangle if EACH of the DISTANCES is less than
// or equal to the HEIGHT of the triangle RELATIVE to the opposite
// vertex
//
// - HEIGHT of the triangle
// - opposite vertex
