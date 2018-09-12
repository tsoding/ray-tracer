#include "./arguments.hpp"

#include <iostream>
#include <string>

Arguments::Arguments(int argc, char **argv):
    m_argc(argc),
    m_argv(argv),
    m_threadCount(1) {
}

std::string Arguments::sceneFile() const {
    return m_sceneFile;
}

std::string Arguments::outputFile() const {
    return m_outputFile;
}

int Arguments::threadCount() const {
    return m_threadCount;
}

bool Arguments::verify() {
    int i = 1;

    for (; i < m_argc; ++i) {
        if (m_argv[i][0] == '-') {
            if (m_argv[i] == std::string("-j")) {
                m_threadCount = std::stoi(m_argv[++i]);
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
              << "<scene-file> "
              << "[output-file] "
              << std::endl;
}