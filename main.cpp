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

using color = vec3<uint8_t>;

using plane = vec<float, 4>;

struct Wall
{
    plane p;
    color c;
};

struct Scene
{
    float sphereZ;
    float sphereR;
    Wall wall;
};

float dot(const plane &p, const vec3<float> &v)
{
    return p.v[0] * v.v[0] + p.v[1] * v.v[1] + p.v[2] * v.v[2] + p.v[3];
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
                fout.put(static_cast<uint8_t>(display[row * width + col].v[k]));
            }
        }
    }
}

color march(float x, float y, const Scene &scene)
{
    const vec3<float> dir = {0.0f, 0.0f, 1.0f};
    const vec3<float> sphereCenter = {0.0f, 0.0f, scene.sphereZ};
    vec3<float> ray = {x, y, 0.0f};

    while (std::abs(dot(scene.wall.p, ray)) > 1e-6) {
        ray += dir;
        if (sqr_norm(sphereCenter - ray) <= scene.sphereR * scene.sphereR) {
            return {0U, 0U, 0U};
        }
    }

    return scene.wall.c;
}

void render_scene(color *display, size_t width, size_t height,
                  const Scene &scene)
{
    const float half_width = static_cast<float>(width) * 0.5f;
    const float half_height = static_cast<float>(height) * 0.5f;

    for (size_t row = 0; row < height; ++row) {
        std::cout << "Row " << row << std::endl;
        for (size_t col = 0; col < width; ++col) {
            display[row * width + col] =
                march(static_cast<float>(col) - half_width,
                      static_cast<float>(row) - half_height,
                      scene);
        }
    }
}

int main(int argc, char *argv[])
{
    const size_t width = 800, height = 600;
    std::array<color, width * height> display;

    render_scene(display.data(), width, height, {
            200,                  // sphereZ
            50,                   // sphereR
            {
                { 0, 0, -1, 300 },    // wall
                { 255, 255, 255 }     // wallColor
            }
        });
    save_display_to_file(display.data(), width, height, "output.ppm");

    return 0;
}
