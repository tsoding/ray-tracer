#include <cmath>
#include <cstdint>

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <numeric>
#include <algorithm>

#include <SFML/Graphics.hpp>

#include "./color.hpp"
#include "./mat4x4.hpp"
#include "./vec.hpp"

template <typename T>
using vec3 = vec<T, 3>;

using plane = vec<float, 4>;

struct Wall {
    plane p;
    color c;
};

std::ostream& operator<<(std::ostream& os, const Wall&wall) {
    return os << "Wall{ p" << wall.p << " c" << wall.c << "}";
}

struct Sphere {
    vec3<float> center;
    float radius;
};

std::ostream& operator<<(std::ostream& os, const Sphere&sphere) {
    return os << "Sphere{ center "
              << sphere.center
              << " radius "
              << sphere.radius
              << "}";
}

struct Cuboid {
    vec3<float> trans;
    vec3<float> rot;
    vec3<float> scale;
    color c;
};

mat4x4<float> inverse_mat_of_cuboid(const Cuboid &cuboid) {
    const std::vector<mat4x4<float>> mts = {
        rot_z_mat(-cuboid.rot.v[2]),
        rot_y_mat(-cuboid.rot.v[1]),
        rot_x_mat(-cuboid.rot.v[0]),
        scale_mat(recip(cuboid.scale)),
        trans_mat(cuboid.trans * -1.0f)
    };

    mat4x4<float> result = id_mat();

    for (const auto &m : mts) {
        result = dot_mat4x4(result, m);
    }

    return result;
}

bool is_point_inside_of_cuboid(const vec3<float> &p,
                               const mat4x4<float> &inverse_mat) {
    const vec3<float> p1 = dot(inverse_mat, p);

    for (size_t i = 0; i < 3; ++i) {
        if (p1.v[i] > 0.5f || p1.v[i] < -0.5f) {
            return false;
        }
    }

    return true;
}

struct Scene {
    vec3<float> eye;
    std::vector<Sphere> spheres;
    std::vector<Wall> walls;
    std::vector<Cuboid> cuboids;
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

float dot(const plane &p, const vec3<float> &v) {
    return p.v[0] * v.v[0] + p.v[1] * v.v[1] + p.v[2] * v.v[2] + p.v[3];
}

float dot(const vec3<float> &v1, const vec3<float> &v2) {
    return v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2];
}

vec3<float> normalize(const vec3<float> &v) {
    return 1.0f / sqrtf(sqr_norm(v)) * v;
}

void save_display_to_file(const color *display,
                          size_t width,
                          size_t height,
                          const std::string &filepath) {
    std::ofstream fout(filepath, std::ofstream::out | std::ostream::binary);

    fout << "P6" << std::endl;
    fout << width << " " << height << std::endl;
    fout << 255 << std::endl;

    for (size_t row = 0; row < height; ++row) {
        for (size_t col = 0; col < width; ++col) {
            for (size_t k = 0; k < 3; ++k) {
                const float x = 255.0f * display[row * width + col].v[k];
                fout.put(static_cast<char>(x));
            }
        }
    }
}

float color_factor(size_t steps, size_t step_count) {
    const float x =
        1.0f - static_cast<float>(steps) / static_cast<float>(step_count);
    return x;
}

color march(float x, float y,
            const Scene &scene,
            const std::vector<mat4x4<float>> &cuboid_inverse_mats,
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
            if (std::abs(dot(wall.p, ray)) <= 0.5f) {
                return wall.c * color_factor(i, step_count);
            }
        }

        // TODO(#38): cuboid is rendered incorrectly
        for (size_t i = 0; i < scene.cuboids.size(); ++i) {
            if (is_point_inside_of_cuboid(ray, cuboid_inverse_mats[i])) {
                return scene.cuboids[i].c * color_factor(i, step_count);
            }
        }
    }

    return {0.0f, 0.0f, 0.0f};
}

void render_scene(color *display, size_t width, size_t height,
                  const Scene &scene) {
    const float half_width = static_cast<float>(width) * 0.5f;
    const float half_height = static_cast<float>(height) * 0.5f;

    std::vector<mat4x4<float>> cuboid_inverse_mats;

    for (const auto &cuboid : scene.cuboids) {
        cuboid_inverse_mats.push_back(inverse_mat_of_cuboid(cuboid));
    }

    for (size_t row = 0; row < height; ++row) {
        // std::cout << "Row " << row << std::endl;
        for (size_t col = 0; col < width; ++col) {
            const vec3<float> p = { static_cast<float>(col) - half_width,
                                    static_cast<float>(row) - half_height,
                                    0.0f };

            display[row * width + col] =
                march(static_cast<float>(col) - half_width,
                      static_cast<float>(row) - half_height,
                      scene,
                      cuboid_inverse_mats,
                      normalize(p - scene.eye));
        }
    }
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
        } else if (type == "c") {  // cuboid
            Cuboid cuboid;
            std::string c;

            iss >> cuboid.trans
                >> cuboid.rot
                >> cuboid.scale
                >> c;
            cuboid.c = color_from_hex(c).value_or(color{1.0f, 1.0f, 1.0f});

            scene.cuboids.push_back(cuboid);
        }
    }

    return scene;
}

int main(int argc, char *argv[]) {
    const size_t width = 800, height = 600;

    if (argc < 3) {
        std::cerr << "./ray-tracer <input-file> <output-file>"
                  << std::endl;
        return -1;
    }

    const std::string input_file(argv[1]);
    const std::string output_file(argv[2]);

    auto scene = load_scene_from_file(input_file);

    std::cout << scene << std::endl;

    std::array<color, width * height> display;
    (void) display;

    sf::RenderWindow window(sf::VideoMode(width, height, 32),
                            "Ray Tracer");
    sf::Texture texture;
    texture.create(width, height);
    sf::Sprite sprite;
    std::array<sf::Uint8, width * height * 4> buffer;

    const float half_width = static_cast<float>(width) * 0.5f;
    const float half_height = static_cast<float>(height) * 0.5f;

    const std::vector<mat4x4<float>> id = {id_mat()};

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        const size_t row = rand() % height;

        for (size_t col = 0; col < width; ++col) {
            const vec3<float> p = { static_cast<float>(col) - half_width,
                                    static_cast<float>(row) - half_height,
                                    0.0f };

            const color pixel_color =
                march(static_cast<float>(col) - half_width,
                      static_cast<float>(row) - half_height,
                      scene,
                      id,
                      normalize(p - scene.eye));

            buffer[row * width * 4 + col * 4 + 0] = static_cast<sf::Uint8>(pixel_color.v[0] * 255.0f);
            buffer[row * width * 4 + col * 4 + 1] = static_cast<sf::Uint8>(pixel_color.v[1] * 255.0f);
            buffer[row * width * 4 + col * 4 + 2] = static_cast<sf::Uint8>(pixel_color.v[2] * 255.0f);
            buffer[row * width * 4 + col * 4 + 3] = 255;

            texture.update(buffer.data());
        }

        sprite.setTexture(texture);

        window.clear();
        window.draw(sprite);
        window.display();
    }

    // render_scene(display.data(), width, height, scene);
    // save_display_to_file(display.data(), width, height, output_file);

    return 0;
}
