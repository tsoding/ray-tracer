#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <string>
#include <vector>
#include <functional>

#include "sphere.hpp"
#include "wall.hpp"
#include "triangle.hpp"
#include "display.hpp"
#include "ray.hpp"

struct Scene {
    vec3<float> eye;
    std::vector<Sphere> spheres;
    std::vector<Wall> walls;
    std::vector<Triangle> triangles;
};

using RenderPixel = std::function<Color(const Scene&, Ray)>;

std::ostream& operator<<(std::ostream& os, const Scene&scene);
const Scene load_scene_from_file(const std::string &filename);
Color march(const Scene &scene, Ray ray);
Color trace(const Scene &scene, Ray ray);
Color debug_sphere(const Scene &scene, Ray ray);

void render_row(const Scene &scene,
                Display *display,
                size_t row,
                RenderPixel renderPixel);

#endif  // SCENE_HPP_
