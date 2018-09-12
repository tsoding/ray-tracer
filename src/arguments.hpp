#ifndef ARGUMENTS_HPP_
#define ARGUMENTS_HPP_

#include <string>

class Arguments {
 public:
    Arguments(int argc, char **argv);

    std::string sceneFile() const;
    std::string outputFile() const;
    size_t threadCount() const;
    size_t width() const;
    size_t height() const;

    bool verify();

    void help() const;

 private:
    const int m_argc;
    char **m_argv;

    std::string m_sceneFile;
    std::string m_outputFile;
    size_t m_threadCount;
    size_t m_width;
    size_t m_height;
};

#endif  // ARGUMENTS_HPP_
