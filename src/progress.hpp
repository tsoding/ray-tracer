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

    // TODO: static_assert message is confusing
    static_assert(std::is_member_function_pointer<decltype(&PartialWork::progressDo)>::value,
                  "PartialWork is expected to have progressDo() method");
    static_assert(std::is_member_function_pointer<decltype(&PartialWork::progressWork)>::value,
                  "PartialWork is expected to have progressWork() method");
    static_assert(std::is_member_function_pointer<decltype(&PartialWork::progressGoal)>::value,
                  "PartialWork is expected to have progressGoal() method");

    Progress(PartialWork &&partialWork,
             const std::string &name):
        m_partialWork(std::move(partialWork)),
        m_name(name) {
    }

    void progressDo() {
        m_partialWork.progressDo();
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
};

#endif  // PROGRESS_HPP_
