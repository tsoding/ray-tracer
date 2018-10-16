#include "ray.hpp"

const float WORLD_HORIZON_FACTOR = 10000.0;
const Color WORLD_SKYBOX_COLOR = Color{0.0f, 0.0f, 0.0f};

Ray void_ray(const Ray &ray)
{
    return {
        ray.origin + WORLD_HORIZON_FACTOR * ray.dir,
        ray.dir,
        WORLD_SKYBOX_COLOR,
        true
    };
}

Ray collide_ray_with_sphere(const Ray &ray, const Sphere &sphere)
{
    // TODO: collide_ray_with_sphere is not implemented
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
        wall.c,
        true
    };
}
