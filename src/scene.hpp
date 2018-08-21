#ifndef SCENE_HPP_
#define SCENE_HPP_

#include "sphere.hpp"
#include "wall.hpp"

struct Scene {
    vec3<float> eye;
    std::vector<Sphere> spheres;
    std::vector<Wall> walls;
};

std::ostream& operator<<(std::ostream& os, const Scene&scene) {
    os << "Scene{" << std::endl
        << "  Eye:" << scene.eye << std::endl;

    for (const auto&sphere : scene.spheres)
          std::cout << "  " << sphere << std::endl;

    for (const auto&wall : scene.walls)
          std::cout << "  " << wall << std::endl;

    return os << "}" << std::endl;
}

#endif  // SCENE_HPP_
