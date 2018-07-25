#ifndef VEC_H_
#define VEC_H_

template <typename T, size_t N>
struct vec {
    T v[N];
};

template <typename T, size_t N>
std::ostream& operator<<(std::ostream& os, const vec<T, N>&vec) {
    os << "{";
    for (size_t i = 0; i < N - 1; i++)
        os << vec.v[i] << ",";
    os << vec.v[N-1] << "}";

    return os;
}

template <typename T, size_t N>
vec<T, N> operator+(const vec<T, N> &v1, const vec<T, N> &v2) {
    vec<T, N> result;

    for (size_t i = 0; i < N; ++i) {
        result.v[i] = v1.v[i] + v2.v[i];
    }

    return result;
}

template <typename T, size_t N>
vec<T, N> operator-(const vec<T, N> &v1, const vec<T, N> &v2) {
    vec<T, N> result;

    for (size_t i = 0; i < N; ++i) {
        result.v[i] = v1.v[i] - v2.v[i];
    }

    return result;
}

template <typename T, size_t N>
vec<T, N> &operator+=(vec<T, N> &v1, const vec<T, N> &v2) {
    for (size_t i = 0; i < N; ++i) {
        v1.v[i] += v2.v[i];
    }

    return v1;
}

template <typename T, size_t N>
vec<T, N> operator*(const T &s, const vec<T, N> &v) {
    vec<T, N> result;

    for (size_t i = 0; i < N; ++i) {
        result.v[i] = v.v[i] * s;
    }

    return result;
}

template <typename T, size_t N>
vec<T, N> operator*(const vec<T, N> &v, const T &s) {
    return s * v;
}

template <typename T, size_t N>
T sqr_norm(const vec<T, N> &v) {
    T acc = T();

    for (size_t i = 0; i < N; ++i) {
        acc += v.v[i] * v.v[i];
    }

    return acc;
}

#endif  // VEC_H_
