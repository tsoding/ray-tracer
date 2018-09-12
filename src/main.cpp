#include <cmath>
#include <cstdint>
#include <cstring>

#include <algorithm>
#include <array>
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

#include "./arguments.hpp"
#include "./color.hpp"
#include "./display.hpp"
#include "./parallel_rendering.hpp"
#include "./progress.hpp"
#include "./row_marching.hpp"
#include "./row_tracing.hpp"
#include "./scene.hpp"
#include "./shuffled_rows.hpp"
#include "./sphere.hpp"
#include "./sprite_display.hpp"
#include "./vec.hpp"

void file_render_mode(const size_t width,
                      const size_t height,
                      const std::string &output_file,
                      const Scene &scene) {
    Display display(width, height);

    mkProgress(
        mkParallelRendering(
            mkRowMarching(
                &scene,
                &display),
            3),
        "Rendering").start();

    display.save_as_ppm(output_file);
}

void preview_mode(const size_t width,
                  const size_t height,
                  const std::string &scene_file) {
    Scene scene = load_scene_from_file(scene_file);
    SpriteDisplay display(width, height);

    auto progress =
        mkProgress(
            mkParallelRendering(
                mkShuffledRows(
                    mkRowMarching(
                        &scene,
                        &display)),
                3),
            "Preview rendering");

    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(width),
                                          static_cast<unsigned int>(height),
                                          32),
                            "Ray Tracer");
    window.setFramerateLimit(0);
    window.setVerticalSyncEnabled(true);
    window.clear(sf::Color(0, 0, 0));
    window.display();

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
                    display.clean();
                    progress.progressReset();
                    break;
                default: break;
                }
                break;
            }
            default: break;
            }
        }

        progress.report();
        progress.progressDo();

        window.clear();
        window.draw(display.sprite());
        window.display();
    }
}

int main(int argc, char *argv[]) {
    const size_t width = 800, height = 600;

    Arguments arguments(argc, argv);

    if (!arguments.verify()) {
        arguments.help();
        return -1;
    }

    if (!arguments.outputFile().empty()) {
        const auto scene = load_scene_from_file(arguments.outputFile());
        file_render_mode(width, height, arguments.outputFile(), scene);
    } else {
        preview_mode(width, height, arguments.sceneFile());
    }

    return 0;
}
