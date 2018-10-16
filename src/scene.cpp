#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "scene.hpp"
#include "ray.hpp"

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

            const auto hex_color = color_from_hex(color_hex);

            scene.walls.push_back({
                    {plane1, plane2, plane3, plane4},
                    hex_color != nullptr ? *hex_color : Color{1.0f, 1.0f, 1.0f}
                });
        } else if (type == "t") {  // triangle
            Triangle t;
            iss >> t.v1 >> t.v2 >> t.v3;
            scene.triangles.push_back(t);
        }
    }

    return scene;
}

static float color_factor(size_t steps, size_t step_count) {
    const float x =
        1.0f - static_cast<float>(steps) / static_cast<float>(step_count);
    return x;
}

Color march(float x, float y,
            const Scene &scene,
            vec3<float> dir) {
    vec3<float> ray = {x, y, 0.0f};
    size_t step_count = 600;

    Color triangle_color = {1.0f, 0.0f, 0.0f};

    for (size_t i = 0; i < step_count; ++i) {
        ray += dir;

        for (const auto &sphere : scene.spheres) {
            if (is_ray_inside_of_sphere(sphere, ray)) {
                vec3<float> norm = normalize(ray - sphere.center);
                dir = normalize(dir - (2 * dot(dir, norm)) * norm);
            }
        }

        for (const auto &wall : scene.walls) {
            if (is_ray_behind_wall(wall, ray)) {
                return wall.c * color_factor(i, step_count);
            }
        }

        for (const auto &triangle : scene.triangles) {
            if (ray_hits_triangle(triangle, ray)) {
                return triangle_color * color_factor(i, step_count);
            }
        }
    }

    return {0.0f, 0.0f, 0.0f};
}

Color trace(float x, float y, const Scene &scene, const vec3<float> &dir) {
    Ray ray = {
       {x, y, 0.0f},
       dir,
       Color{1.0f, 1.0f, 1.0f},
       false
    };

    while (!ray.absorbed) {
        Ray next_ray = void_ray(ray);

        for (const auto &wall: scene.walls) {
            const Ray wall_ray = collide_ray_with_wall(ray, wall);

            if (len(ray.origin - wall_ray.origin) < len(ray.origin - next_ray.origin)) {
                next_ray = wall_ray;
            }
        }

        for (const auto &sphere: scene.spheres) {
            const Ray sphere_ray = collide_ray_with_sphere(ray, sphere);

            if (len(ray.origin - sphere_ray.origin) < len(ray.origin - next_ray.origin)) {
                next_ray = sphere_ray;
            }
        }

        ray = next_ray;
    }

    // TODO(#97): spheres do not reflect the rays
    // TODO(#89): trace() does not support triangles

    return ray.color;
}
