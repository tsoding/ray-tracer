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

            const auto hex_color = color_from_hex(color_hex);

            scene.walls.push_back({
                    {plane1, plane2, plane3, plane4},
                    hex_color != nullptr ? *hex_color : color{1.0f, 1.0f, 1.0f}
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

color march(float x, float y,
            const Scene &scene,
            vec3<float> dir) {
    vec3<float> ray = {x, y, 0.0f};
    size_t step_count = 600;

    color triangle_color = {1.0f, 0.0f, 0.0f};

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

// TODO(#85): solve_plane_line is not a very descriptive name
float solve_plane_line(const plane &p,
                       const v3f &line_start,
                       const v3f &line_direction) {
    const float A = p.v[0], B = p.v[1], C = p.v[2], D = p.v[3];
    const float x1 = line_start.v[0], y1 = line_start.v[1], z1 = line_start.v[2];
    const float dx = line_direction.v[0], dy = line_direction.v[1], dz = line_direction.v[2];
    // TODO(#86): possible division by zero in solve_plane_line
    const float t = - (A * x1 + B * y1 + C * z1 + D) / (A * dx + B * dy + C * dz);
    return t;
}

color trace(float x, float y, const Scene &scene, const vec3<float> &dir) {
    const vec3<float> ray = {x, y, 0.0f};

    bool found = false;
    vec3<float> closest_point;
    color closest_color = {0.0f, 0.0f, 0.0f};

    for (size_t i = 0; i < scene.walls.size(); ++i) {
        const float t = solve_plane_line(scene.walls[i].p, ray, dir);

        if (t < 0.0f) {
            continue;
        }

        if (!found) {
            found = true;
            closest_point = ray + t * dir;
            closest_color = scene.walls[i].c;
        } else {
            const vec3<float> new_closest_point = ray + t * dir;
            if (len(ray - new_closest_point) < len(ray - closest_point)) {
                closest_point = new_closest_point;
                closest_color = scene.walls[i].c;
            }
        }
    }

    // TODO(#87): trace() does not support shadows
    // TODO(#88): trace() does not support spheres
    // TODO(#89): trace() does not support triangles

    return closest_color;
}
