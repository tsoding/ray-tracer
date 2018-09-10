#ifndef PROGRESS_HPP_
#define PROGRESS_HPP_

#include <iostream>
#include <string>
#include <type_traits>

template <typename T>
class Progress
{
 public:
    using PartialWork = T;

    Progress(PartialWork &&partialWork,
             const std::string &name,
             size_t batch = 1):
        m_partialWork(std::move(partialWork)),
        m_name(name),
        m_batch(batch) {
    }

    void progressDo() {
        for (size_t i = 0; i < m_batch; ++i) {
            m_partialWork.progressDo();
        }
    }

    void progressReset() {
        m_partialWork.progressReset();
    }

    size_t progressGoal() const {
        return m_partialWork.progressGoal();
    }

    size_t progressWork() const {
        return m_partialWork.progressWork();
    }

    void report() const {
        const auto work = m_partialWork.progressWork();
        const auto goal = m_partialWork.progressGoal();
        const float progress =
            static_cast<float>(work) / static_cast<float>(goal) * 100.0f;
        std::cout << "\r" << m_name << "... "
                  << std::fixed << std::setprecision(1)
                  << progress
                  << std::left << std::setfill(' ') << std::setw(2)
                  << "%" << std::flush;
    }

    void start() {
        while (m_partialWork.progressWork() < m_partialWork.progressGoal()) {
            m_partialWork.progressDo();
            report();
        }
    }

 private:
    PartialWork m_partialWork;
    const std::string m_name;
    const size_t m_batch;
};

template <typename PartialWork>
inline Progress<PartialWork>
mkProgress(PartialWork &&partialWork,
           const std::string &name,
           size_t batch = 1) {
    return Progress<PartialWork>(std::move(partialWork), name, batch);
}

#endif  // PROGRESS_HPP_
