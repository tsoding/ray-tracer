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

void file_render_mode(const Arguments &arguments) {
    Display display(arguments.width(), arguments.height());
    Scene scene = load_scene_from_file(arguments.sceneFile());

    mkProgress(
        mkParallelRendering(
            mkRowMarching(
                &scene,
                &display),
            arguments.threadCount()),
        "Rendering").start();

    display.save_as_ppm(arguments.outputFile());
}

void preview_mode(const Arguments &arguments) {
    Scene scene = load_scene_from_file(arguments.sceneFile());
    SpriteDisplay display(arguments.width(), arguments.height());

    auto progress =
        mkProgress(
            mkParallelRendering(
                mkShuffledRows(
                    mkRowMarching(
                        &scene,
                        &display)),
                arguments.threadCount()),
            "Preview rendering");

    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(arguments.width()),
                                          static_cast<unsigned int>(arguments.height()),
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
                    scene = load_scene_from_file(arguments.sceneFile());
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
    Arguments arguments(argc, argv);

    if (!arguments.verify()) {
        arguments.help();
        return -1;
    }

    if (!arguments.outputFile().empty()) {
        file_render_mode(arguments);
    } else {
        preview_mode(arguments);
    }

    return 0;
}
