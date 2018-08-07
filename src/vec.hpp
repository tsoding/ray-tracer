#ifndef SRC_VEC_HPP_
#define SRC_VEC_HPP_

template <typename T, size_t N>
struct vec {
    T v[N];
};

template <typename T, size_t N>
vec<T, N> recip(const vec<T, N> &v) {
    vec<T, N> r;
    for (size_t i = 0; i < N; ++i) {
        if (v.v[i] == 0.0f) {
            r.v[i] = 0.0f;
        } else {
            r.v[i] = 1 / v.v[i];
        }
    }
    return r;
}

template <typename T, size_t N>
std::ostream& operator<<(std::ostream& os, const vec<T, N> &vec) {
    os << "{";
    for (size_t i = 0; i < N - 1; i++)
        os << vec.v[i] << ",";
    os << vec.v[N-1] << "}";

    return os;
}

template <typename T, size_t N>
std::istream& operator>>(std::istream& is, vec<T, N> &vec) {
    for (size_t i = 0; i < N; ++i) {
        is >> vec.v[i];
    }
    return is;
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
vec<T, N> &operator+=(vec<T, N> &v1,  // NOLINT(runtime/references)
                      const vec<T, N> &v2) {
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

#endif  // SRC_VEC_HPP_
