#include "./arguments.hpp"

#include <iostream>
#include <string>

Arguments::Arguments(int argc, char **argv):
    m_argc(argc),
    m_argv(argv),
    m_threadCount(1),
    m_width(800),
    m_height(600) {
}

std::string Arguments::sceneFile() const {
    return m_sceneFile;
}

std::string Arguments::outputFile() const {
    return m_outputFile;
}

size_t Arguments::threadCount() const {
    return m_threadCount;
}

size_t Arguments::width() const {
    return m_width;
}

size_t Arguments::height() const {
    return m_height;
}

bool Arguments::verify() {
    int i = 1;

    for (; i < m_argc; ++i) {
        if (m_argv[i][0] == '-') {
            if (m_argv[i] == std::string("-j")) {
                // TODO: Arguments option parsing doesn't check if the parameter of option is available
                m_threadCount = std::stoul(m_argv[++i]);
            } else if (m_argv[i] == std::string("-w")) {
                m_width = std::stoul(m_argv[++i]);
            } else if (m_argv[i] == std::string("-h")) {
                m_height = std::stoul(m_argv[++i]);
            } else {
                std::cerr << "Unexpected option: "
                          << m_argv[i]
                          << std::endl;
                return false;
            }
        } else {
            break;
        }
    }

    if (i < m_argc) {
        m_sceneFile = m_argv[i++];
    } else {
        std::cerr << "Expected scene file"
                  << std::endl;
        return false;
    }

    if (i < m_argc) {
        m_outputFile = m_argv[i++];
    }

    return true;
}

void Arguments::help() const {
    std::cerr << "./ray-tracer "
              << "[-j <thread-count>] "
              << "[-w <width>] "
              << "[-h <height>]"
              << "<scene-file> "
              << "[output-file] "
              << std::endl;
}
