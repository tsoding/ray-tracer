#ifndef ARGUMENTS_HPP_
#define ARGUMENTS_HPP_

#include <string>

class Arguments {
 public:
    Arguments(int argc, char **argv);

    std::string sceneFile() const;
    std::string outputFile() const;
    int threadCount() const;

    bool verify();

    void help() const;

 private:
    const int m_argc;
    char **m_argv;

    std::string m_sceneFile;
    std::string m_outputFile;
    int m_threadCount;
};

#endif  // ARGUMENTS_HPP_
