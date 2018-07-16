#include <cmath>
#include <cstdint>

#include <iostream>
#include <array>
#include <fstream>
#include <string>

template <typename T>
struct vec3
{
    T v[3];
};

using color = vec3<uint8_t>;

template <typename T>
vec3<T> operator+(const vec3<T> &v1, const vec3<T> &v2)
{
    return {
        v1.v[0] + v2.v[0],
        v1.v[1] + v2.v[1],
        v1.v[2] + v2.v[2]
    };
}

template <typename T>
vec3<T> operator-(const vec3<T> &v1, const vec3<T> &v2)
{
    return {
        v1.v[0] - v2.v[0],
        v1.v[1] - v2.v[1],
        v1.v[2] - v2.v[2]
    };
}

template <typename T>
vec3<T> &operator+=(vec3<T> &v1, const vec3<T> &v2)
{
    v1.v[0] += v2.v[0];
    v1.v[1] += v2.v[1];
    v1.v[2] += v2.v[2];
    return v1;
}

float sqr_norm(const vec3<float> &v)
{
    return v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2];
}

struct Scene
{
    float sphereZ;
    float sphereR;
    float lightZ;
    color lightColor;
};

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

    while (ray.v[2] <= scene.lightZ) {
        ray += dir;
        if(sqr_norm(sphereCenter - ray) <= scene.sphereR * scene.sphereR) {
            return {0U, 0U, 0U};
        }
    }

    return scene.lightColor;
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

    render_scene(display.data(), width, height, {200, 50, 300, {255, 255, 255}});
    save_display_to_file(display.data(), width, height, "output.ppm");

    return 0;
}
