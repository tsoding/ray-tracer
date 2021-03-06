#ifndef RAY_HPP_
#define RAY_HPP_

#include "vec.hpp"
#include "color.hpp"
#include "sphere.hpp"
#include "wall.hpp"

struct Ray {
    v3f origin;
    v3f dir;
    Color color;
    bool absorbed;
};

inline Ray ray(const v3f &origin, const v3f &dir, const Color &color) {
    return {origin, dir, color, false};
}

inline Ray closer_ray(Ray source, Ray r1, Ray r2) {
    if (len(source.origin - r1.origin) < len(source.origin - r2.origin)) {
        return r1;
    } else {
        return r2;
    }
}

Ray void_ray(const Ray &ray);
Ray collide_ray_with_sphere(const Ray &ray, const Sphere &sphere);
Ray collide_ray_with_wall(const Ray &ray, const Wall &wall);

#endif  // RAY_HPP_
