#include <algorithm>

#include "ray.hpp"

const float WORLD_HORIZON_FACTOR = 10000.0f;
const Color WORLD_SKYBOX_COLOR = Color{0.0f, 0.0f, 0.0f};
const float MAX_RAY_LENGTH = 600.0f;

template <typename T>
static T sqr(T x) {
    return x * x;
}

// https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
static float disc(const v3f &o, const v3f &l,  // line
           const v3f &c, float r) {     // sphere
    return sqr(dot(l, o - c)) - sqr(len(o - c)) + sqr(r);
}

Ray void_ray(const Ray &ray) {
    return {
        ray.origin + WORLD_HORIZON_FACTOR * ray.dir,
        ray.dir,
        WORLD_SKYBOX_COLOR,
        true
    };
}

Ray absorb_ray(const Ray &ray, const Color &color) {
    return {
        ray.origin,
        ray.dir,
        color,
        true
    };
}

Ray collide_ray_with_sphere(const Ray &ray, const Sphere &sphere) {
    const float d = disc(ray.origin, ray.dir,
                         sphere.center,
                         sphere.radius);

    if (d >= 0.0f) {
        return absorb_ray(ray, Color{0.0f, 0.0f, 1.0f});
    }

    return void_ray(ray);
}

Ray collide_ray_with_wall(const Ray &ray, const Wall &wall) {
    const float b = dot(vec_pop(wall.p), ray.dir);

    // They ray is moving along with the plane
    if (std::fabs(b) < 1e-6) {
        return void_ray(ray);
    }

    const float a = -dot(wall.p, vec_push(ray.origin, 1.0f));
    const float t = a / b;

    // Line intersects the plane but direction points away from the plane
    if (t < 0.0f) {
        return void_ray(ray);
    }

    return {
        ray.origin + t * ray.dir,
        ray.dir,
        wall.c * (1.0f - std::min(MAX_RAY_LENGTH, t) / MAX_RAY_LENGTH),
        true
    };
}
