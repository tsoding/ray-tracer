#ifndef SRC_MAT4X4_HPP_
#define SRC_MAT4X4_HPP_

#include <cmath>

#include "./vec.hpp"

template <typename T>
using mat4x4 = vec<T, 4 * 4>;

mat4x4<float> dot_mat4x4(const mat4x4<float> &m1, const mat4x4<float> &m2) {
    mat4x4<float> r;

    for (size_t i = 0; i < 4; ++i) {
        for (size_t j = 0; j < 4; ++j) {
            r.v[i * 4 + j] = 0;
            for (size_t k = 0; k < 4; ++k) {
                r.v[i * 4 + j] += m1.v[i * 4 + k] * m2.v[k * 4 + j];
            }
        }
    }

    return r;
}

vec<float, 3> dot(const mat4x4<float> &m, const vec<float, 3> &v) {
    const vec<float, 4> homo_v = {v.v[0], v.v[1], v.v[2], 1.0f};
    vec<float, 4> r;

    for (size_t i = 0; i < 4; ++i) {
        r.v[i] = 0.0f;
        for (size_t j = 0; j < 4; ++j) {
            r.v[i] += homo_v.v[i] * m.v[i * 4 + j];
        }
    }

    return {
        r.v[0] / r.v[3],
        r.v[1] / r.v[3],
        r.v[2] / r.v[3]
    };
}

mat4x4<float> id_mat() {
    return {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

template <typename T>
mat4x4<T> trans_mat(const vec<T, 3> &v) {
    return {
        1.0f, 0.0f, 0.0f, v.v[0],
        0.0f, 1.0f, 0.0f, v.v[1],
        0.0f, 0.0f, 1.0f, v.v[2],
        0.0f, 0.0f, 0.0f, 1.0f
    };
}

mat4x4<float> rot_x_mat(float a) {
    using std::cos;
    using std::sin;

    return {
        1.0f, 0.0f,    0.0f,   0.0f,
        0.0f, cos(a), -sin(a), 0.0f,
        0.0f, sin(a),  cos(a), 0.0f,
        0.0f, 0.0f,    0.0f,   1.0f
    };
}

mat4x4<float> rot_y_mat(float a) {
    using std::cos;
    using std::sin;

    return {
        cos(a), 0.0f, sin(a), 0.0f,
        0.0f,   1.0f, 0.0f,   0.0f,
       -sin(a), 0.0f, cos(a), 0.0f,
        0.0f,   0.0f, 0.0f,   1.0f
    };
}

mat4x4<float> rot_z_mat(float a) {
    using std::cos;
    using std::sin;

    return {
        cos(a), -sin(a), 0.0f, 0.0f,
        sin(a),  cos(a), 0.0f, 0.0f,
        0.0f,    0.0f,   1.0f, 0.0f,
        0.0f,    0.0f,   0.0f, 1.0f
    };
}

mat4x4<float> scale_mat(vec<float, 3> v) {
    return {
        v.v[0], 0.0f,   0.0f,   0.0f,
        0.0f,   v.v[1], 0.0f,   0.0f,
        0.0f,   0.0f,   v.v[2], 0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    };
}

#endif  // SRC_MAT4X4_HPP_
