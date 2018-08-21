#include <cmath>
#include <cstdint>
#include <cstring>

#include <algorithm>
#include <array>
#include <optional>             // NOLINT
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>    // NOLINT

#include "./color.hpp"
#include "./ppm.hpp"
#include "./scene.hpp"
#include "./sphere.hpp"
#include "./vec.hpp"

void file_render_mode(const size_t width,
                      const size_t height,
                      const std::string &output_file,
                      const Scene &scene) {
    std::unique_ptr<color[]> display(new color[width * height]);

    const float half_width = static_cast<float>(width) * 0.5f;
    const float half_height = static_cast<float>(height) * 0.5f;

    for (size_t row = 0; row < height; ++row) {
        std::cout << "\rRendering... "
            << std::fixed << std::setprecision(1)
            << static_cast<float>(100 * row) / static_cast<float>(height)
            << std::left << std::setfill(' ') << std::setw(2)
            << "%" << std::flush;

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
    std::cout << "\rRendering... 100.0%" << std::endl;

    save_ppm_file(display.get(), width, height, output_file);
}

void preview_mode(const size_t width,
                  const size_t height,
                  const std::string &scene_file) {
    std::mt19937 gen { std::random_device {}() };
    std::vector<size_t> ns(height);
    std::iota(ns.begin(), ns.end(), 0);

    Scene scene = load_scene_from_file(scene_file);

    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(width),
                                          static_cast<unsigned int>(height),
                                          32),
                            "Ray Tracer");
    window.setFramerateLimit(0);
    window.setVerticalSyncEnabled(true);
    window.clear(sf::Color(0, 0, 0));
    window.display();

    const size_t textureSize = width * height * 4;

    std::vector<sf::Uint8> buffer(textureSize, 0);

    sf::Texture texture;
    texture.create(static_cast<unsigned int>(width), static_cast<unsigned int>(height));

    sf::Sprite sprite(texture, sf::IntRect(0, 0, static_cast<int>(width), static_cast<int>(height)));

    const float half_width = static_cast<float>(width) * 0.5f;
    const float half_height = static_cast<float>(height) * 0.5f;

    size_t k = 0, i = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed: {
                window.close();
                break;
            }
            case sf::Event::Resized: {
                sf::Vector2f size = static_cast<sf::Vector2f>(window.getSize());
                window.setView(sf::View(sf::FloatRect(0.f, 0.f, size.x, size.y)));
                break;
            }
            case sf::Event::KeyPressed: {
                switch (event.key.code) {
                case sf::Keyboard::Q:
                case sf::Keyboard::Escape:
                    window.close();
                    break;
                case sf::Keyboard::R:
                    scene = load_scene_from_file(scene_file);
                    std::fill(buffer.begin(), buffer.end(), 0);
                    i = 0;
                    break;
                default: break;
                }
                break;
            }
            default: break;
            }
        }

        if (i < ns.size()) {
            if (i == 0) {
                std::shuffle(ns.begin(), ns.end(), gen);
                std::cout << "\rRendering..." << std::flush;
            }

            std::cout << "\rRendering... "
                << std::fixed << std::setprecision(1)
                << static_cast<float>(100*i)/static_cast<float>(height)
                << std::left << std::setfill(' ') << std::setw(2)
                << "%" << std::flush;

            const size_t row = ns[i++];

            for (size_t col = 0; col < width; ++col) {
                const vec3<float> ray = { static_cast<float>(col) - half_width,
                                          static_cast<float>(row) - half_height,
                                          0.0f };
                const color pixel_color =
                    march(static_cast<float>(col) - half_width,
                          static_cast<float>(row) - half_height,
                          scene,
                          normalize(ray - scene.eye));

                buffer[row * width * 4 + col * 4 + 0] = static_cast<sf::Uint8>(pixel_color.v[0] * 255.0f);
                buffer[row * width * 4 + col * 4 + 1] = static_cast<sf::Uint8>(pixel_color.v[1] * 255.0f);
                buffer[row * width * 4 + col * 4 + 2] = static_cast<sf::Uint8>(pixel_color.v[2] * 255.0f);
                buffer[row * width * 4 + col * 4 + 3] = 255;
            }
        } else if (i == ns.size()) {
            ++i;
            std::cout << "\rRendering... 100.0%" << std::endl;
        }

        if (k == 0) {
            texture.update(buffer.data());
            sprite.setTexture(texture);

            window.clear();
            window.draw(sprite);
            window.display();
        }

        k = (k + 1) % 10;
    }
}

int main(int argc, char *argv[]) {
    const size_t width = 800, height = 600;

    if (argc < 2) {
        std::cerr << "./ray-tracer <scene-file> [output-file]"
                  << std::endl;
        return -1;
    }

    const std::string scene_file(argv[1]);
    const std::string output_file =
        argc >= 3
        ? std::string(argv[2])
        : std::string();

    if (!output_file.empty()) {
        const auto scene = load_scene_from_file(scene_file);
        file_render_mode(width, height, output_file, scene);
    } else {
        preview_mode(width, height, scene_file);
    }

    return 0;
}
