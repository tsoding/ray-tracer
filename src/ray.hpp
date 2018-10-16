#ifndef RAY_HPP_
#define RAY_HPP_

#include "vec.hpp"
#include "color.hpp"
#include "sphere.hpp"
#include "wall.hpp"

struct Ray
{
    v3f origin;
    v3f dir;
    Color color;
    bool absorbed;
};

Ray void_ray(const Ray &ray);
Ray collide_ray_with_sphere(const Ray &ray, const Sphere &sphere);
Ray collide_ray_with_wall(const Ray &ray, const Wall &wall);

#endif  // RAY_HPP_
