#include <cmath>
#include <cstdint>
#include <cstring>
#include <cassert>

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
#include "./scene.hpp"
#include "./sphere.hpp"
#include "./vec.hpp"

template <typename T>
void report_progress(const std::string &title,
                     T work, T goal) {
    const float progress =
        static_cast<float>(work) / static_cast<float>(goal) * 100.0f;

    std::cout << "\r" << title << "... "
              << std::fixed << std::setprecision(1)
              << progress
              << std::left << std::setfill(' ') << std::setw(2)
              << "%" << std::flush;
}

void file_render_mode(const Arguments &arguments) {
    Display display(arguments.width(), arguments.height());
    Scene scene = load_scene_from_file(arguments.sceneFile());

    for (size_t row = 0; row < display.height; ++row) {
        render_row(scene, &display, row, trace);
        report_progress("Rendering", row, display.height - 1);
    }

    save_as_ppm(display, arguments.outputFile());
}

void preview_mode(const Arguments &arguments) {
    // TODO: Preview Mode is not implemented (use SDL please)
    assert(!"Preview Mode is not implemented");
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
