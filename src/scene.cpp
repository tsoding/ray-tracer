#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "scene.hpp"

std::ostream& operator<<(std::ostream& os, const Scene&scene) {
    os << "Scene{" << std::endl
        << "  Eye:" << scene.eye << std::endl;

    for (const auto&sphere : scene.spheres)
          std::cout << "  " << sphere << std::endl;

    for (const auto&wall : scene.walls)
          std::cout << "  " << wall << std::endl;

    return os << "}" << std::endl;
}

const Scene load_scene_from_file(const std::string &filename) {
    std::ifstream infile(filename);
    Scene scene = {};
    std::string type, line;

    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        iss >> type;

        if (type == "e") {  // eye
            float eye1, eye2, eye3;
            iss >> eye1 >> eye2 >> eye3;
            scene.eye = { eye1, eye2, eye3 };
        } else if (type == "s") {  // sphere
            float c1, c2, c3, radius;
            iss >> c1 >> c2 >> c3 >> radius;
            scene.spheres.push_back({ {c1, c2, c3}, radius });
        } else if (type == "w") {  // walls
            float plane1, plane2, plane3, plane4;
            std::string color_hex;
            iss >> plane1 >> plane2 >> plane3 >> plane4 >> color_hex;

            scene.walls.push_back({
                    {plane1, plane2, plane3, plane4},
                    color_from_hex(color_hex).value_or(color{1.0f, 1.0f, 1.0f})
                });
        }
    }

    return scene;
}

static float color_factor(size_t steps, size_t step_count) {
    const float x =
        1.0f - static_cast<float>(steps) / static_cast<float>(step_count);
    return x;
}

color march(float x, float y,
            const Scene &scene,
            vec3<float> dir) {
    vec3<float> ray = {x, y, 0.0f};
    size_t step_count = 600;

    for (size_t i = 0; i < step_count; ++i) {
        ray += dir;

        for (const auto &sphere : scene.spheres) {
            if (sqr_norm(sphere.center - ray) < sphere.radius * sphere.radius) {
                vec3<float> norm = normalize(ray - sphere.center);
                dir = normalize(dir - (2 * dot(dir, norm)) * norm);
            }
        }

        for (const auto &wall : scene.walls) {
            if (is_ray_behind_wall(wall, ray)) {
                return wall.c * color_factor(i, step_count);
            }
        }
    }

    return {0.0f, 0.0f, 0.0f};
}
