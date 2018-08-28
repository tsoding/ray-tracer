#ifndef PLANE_HPP_
#define PLANE_HPP_

using plane = vec<float, 4>;

inline float dot(const plane &p, const vec3<float> &v) {
    return p.v[0] * v.v[0] + p.v[1] * v.v[1] + p.v[2] * v.v[2] + p.v[3];
}

#endif  // PLANE_HPP_
