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
    const plane triangle_plane = plane_of_triangle(triangle);

    const float h1 = point_segment_distance(triangle.v1, triangle.v2, triangle.v3);
    const float d1 = sqrtf(sqr_norm(ray - triangle.v1));

    const float h2 = point_segment_distance(triangle.v2, triangle.v3, triangle.v1);
    const float d2 = sqrtf(sqr_norm(ray - triangle.v2));

    const float h3 = point_segment_distance(triangle.v3, triangle.v1, triangle.v2);
    const float d3 = sqrtf(sqr_norm(ray - triangle.v3));

    return dot(vec_push(ray, 1.0f), triangle_plane) <= 0.0f && d1 <= h1 && d2 <= h2 && d3 <= h3;
}

// The point is in the triangle if EACH of the DISTANCES is less than
// or equal to the HEIGHT of the triangle RELATIVE to the opposite
// vertex
//
// - HEIGHT of the triangle
// - opposite vertex
