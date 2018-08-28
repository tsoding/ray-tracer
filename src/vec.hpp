#ifndef SRC_VEC_HPP_
#define SRC_VEC_HPP_

#include <ostream>
#include <cmath>

template <typename T, size_t N>
struct vec {
    T v[N];
};

template <typename T>
using vec3 = vec<T, 3>;

using v3f = vec3<float>;

template <typename T, size_t N>
inline std::ostream& operator<<(std::ostream& os, const vec<T, N> &vec) {
    os << "{";
    for (size_t i = 0; i < N - 1; i++)
        os << vec.v[i] << ",";
    os << vec.v[N-1] << "}";

    return os;
}

template <typename T, size_t N>
inline std::istream& operator>>(std::istream& is, vec<T, N> &vec) {
    for (size_t i = 0; i < N; ++i) {
        is >> vec.v[i];
    }
    return is;
}

template <typename T, size_t N>
inline vec<T, N> operator+(const vec<T, N> &v1, const vec<T, N> &v2) {
    vec<T, N> result;

    for (size_t i = 0; i < N; ++i) {
        result.v[i] = v1.v[i] + v2.v[i];
    }

    return result;
}

template <typename T, size_t N>
inline vec<T, N> operator-(const vec<T, N> &v1, const vec<T, N> &v2) {
    vec<T, N> result;

    for (size_t i = 0; i < N; ++i) {
        result.v[i] = v1.v[i] - v2.v[i];
    }

    return result;
}

template <typename T, size_t N>
inline vec<T, N> &operator+=(vec<T, N> &v1,  // NOLINT(runtime/references)
                      const vec<T, N> &v2) {
    for (size_t i = 0; i < N; ++i) {
        v1.v[i] += v2.v[i];
    }

    return v1;
}

template <typename T, size_t N>
inline vec<T, N> operator*(const T &s, const vec<T, N> &v) {
    vec<T, N> result;

    for (size_t i = 0; i < N; ++i) {
        result.v[i] = v.v[i] * s;
    }

    return result;
}

template <typename T, size_t N>
inline vec<T, N> operator*(const vec<T, N> &v, const T &s) {
    return s * v;
}

template <typename T, size_t N>
inline T sqr_norm(const vec<T, N> &v) {
    T acc = T();

    for (size_t i = 0; i < N; ++i) {
        acc += v.v[i] * v.v[i];
    }

    return acc;
}

template <typename T, size_t N>
inline T dot(const vec<T, N> &v1, const vec<T, N> &v2) {
    T result = T();

    for (size_t i = 0; i < N; ++i) {
        result += v1.v[i] * v2.v[i];
    }

    return result;
}

inline vec3<float> normalize(const vec3<float> &v) {
    return 1.0f / sqrtf(sqr_norm(v)) * v;
}

template <typename T, size_t N>
inline vec<T, N + 1> vec_push(const vec<T, N> &v1, const T &t) {
    vec<T, N + 1> v2;

    for (size_t i = 0; i < N; ++i) {
        v2.v[i] = v1.v[i];
    }

    v2.v[N] = t;

    return v2;
}

template <typename T, size_t N>
inline typename std::enable_if<(N > 0), vec<T, N - 1>>::type
vec_pop(const vec<T, N> &v1) {
    vec<T, N - 1> v2;

    for (size_t i = 0; i < N - 1; ++i) {
        v2.v[i] = v1.v[i];
    }

    return v2;
}

#endif  // SRC_VEC_HPP_
