#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <string>
#include <vector>

#include "sphere.hpp"
#include "wall.hpp"
#include "triangle.hpp"

struct Scene {
    vec3<float> eye;
    std::vector<Sphere> spheres;
    std::vector<Wall> walls;
    std::vector<Triangle> triangles;
};

std::ostream& operator<<(std::ostream& os, const Scene&scene);
const Scene load_scene_from_file(const std::string &filename);
color march(float x, float y, const Scene &scene, vec3<float> dir);
color trace(float x, float y,
            const Scene &scene,
            const vec3<float> &dir);

#endif  // SCENE_HPP_
