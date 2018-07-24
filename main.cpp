#include <cmath>
#include <cstdint>

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "vec.hpp"

template <typename T>
using vec3 = vec<T, 3>;

using color = vec3<float>;

using plane = vec<float, 4>;

struct Wall
{
    plane p;
    color c;
};

struct Sphere
{
    vec3<float> center;
    float radius;
};

struct Scene
{
    vec3<float> eye;
    std::vector<Sphere> spheres;
    std::vector<Wall> walls;
};

float dot(const plane &p, const vec3<float> &v)
{
    return p.v[0] * v.v[0] + p.v[1] * v.v[1] + p.v[2] * v.v[2] + p.v[3];
}

float dot(const vec3<float> &v1, const vec3<float> &v2)
{
    return v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2];
}

vec3<float> normalize(const vec3<float> &v)
{
    return 1.0f / sqrtf(sqr_norm(v)) * v;
}

void save_display_to_file(const color *display,
                          size_t width,
                          size_t height,
                          const std::string &filepath)
{
    std::ofstream fout(filepath, std::ofstream::out | std::ostream::binary);

    fout << "P6" << std::endl;
    fout << width << " " << height << std::endl;
    fout << 255 << std::endl;

    for (size_t row = 0; row < height; ++row) {
        for (size_t col = 0; col < width; ++col) {
            for (size_t k = 0; k < 3; ++k) {
                fout.put(static_cast<char>(255.0f * display[row * width + col].v[k]));
            }
        }
    }
}

color march(float x, float y,
            const Scene &scene,
            vec3<float> dir)
{
    vec3<float> ray = {x, y, 0.0f};
    size_t step_count = 5000;

    for (size_t i = 0; i < step_count; ++i) {
        ray += dir;

        for (const auto &sphere: scene.spheres) {
            if (sqr_norm(sphere.center - ray) < sphere.radius * sphere.radius) {
                vec3<float> norm = normalize(ray - sphere.center);
                dir = normalize(dir - (2 * dot(dir, norm)) * norm);
            }
        }

        for (const auto &wall: scene.walls) {
            if (std::abs(dot(wall.p, ray)) <= 0.5f) {
                return wall.c;
            }
        }
    }

    return {0.0f, 0.0f, 0.0f};
}

void render_scene(color *display, size_t width, size_t height,
                  const Scene &scene)
{
    const float half_width = static_cast<float>(width) * 0.5f;
    const float half_height = static_cast<float>(height) * 0.5f;

    for (size_t row = 0; row < height; ++row) {
        std::cout << "Row " << row << std::endl;
        for (size_t col = 0; col < width; ++col) {
            const vec3<float> p = { static_cast<float>(col) - half_width,
                                    static_cast<float>(row) - half_height,
                                    0.0f };

            display[row * width + col] =
                march(static_cast<float>(col) - half_width,
                      static_cast<float>(row) - half_height,
                      scene,
                      normalize(p - scene.eye));
        }
    }
}

int main(int argc, char *argv[])
{
    const size_t width = 800, height = 600;
    std::array<color, width * height> display;

    if (argc < 4) {
        std::cerr << "./ray-tracer <sphere-x> <sphere-y> <file-name>" << std::endl;
        return -1;
    }

    const float x = static_cast<float>(std::atof(argv[1]));
    const float y = static_cast<float>(std::atof(argv[2]));
    const std::string file_name(argv[3]);
    render_scene(display.data(), width, height, {
            { 0.0f, 0.0f, -200.0f },       // eye
            {                              // spheres
                { { x, y, 200.0f }, 100.0f },
                { { x + 100.0f, y, 100.0f }, 100.0f }
            },
            {                              // walls
                {
                    { 0, 0, -1, 500 },    // p
                    { 1.0f, 1.0f, 1.0f }     // c
                },
                {
                    { 0, 0, 1, 500 },     // p
                    { 1.0f, 0, 0 }         // c
                },
                {
                    { 1, 0, 0, 500 },    // p
                    { 0, 1.0f, 0 }        // c
                },
                {
                    { -1, 0, 0, 500 },    // p
                    { 0, 0, 1.0f }        // c
                },
                {
                    { 0, 1, 0, 500 },    // p
                    { 1.0f, 1.0f, 0 }        // c
                },
                {
                    { 0, -1, 0, 500 },    // p
                    { 1.0f, 0, 1.0f }        // c
                }
            }
        });
    save_display_to_file(display.data(), width, height, file_name);

    return 0;
}
