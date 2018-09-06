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
    using PartialWorkUnit = decltype(PartialWork::progressGoal());

    // TODO: static_assert message is confusing
    static_assert(std::is_member_function_pointer<decltype(&PartialWork::progressWork)>::value,
                  "PartialWork is expected to have progressWork() method");
    static_assert(std::is_member_function_pointer<decltype(&PartialWork::progressGoal)>::value,
                  "PartialWork is expected to have progressGoal() method");

    Progress(PartialWork &&partialWork,
             const std::string &name):
        m_partialWork(std::move(partialWork)),
        m_name(name) {
    }

    PartialWorkUnit progressGoal() {
        return m_partialWork.progressGoal();
    }

    PartialWorkUnit progressWork() {
        return m_partialWork.progressWork();
    }

    void start() {
        PartialWorkUnit work = 0;
        while (work < m_partialWork.progressGoal()) {
            work = m_partialWork.progressWork();
            const float progress =
                static_cast<float>(100 * work) / static_cast<float>(m_partialWork.progressGoal());
            std::cout << "\r" << m_name << "... "
                      << std::fixed << std::setprecision(1)
                      << progress
                      << std::left << std::setfill(' ') << std::setw(2)
                      << "%" << std::flush;
        }
    }

 private:
    PartialWork m_partialWork;
    const std::string m_name;
};

#endif  // PROGRESS_HPP_
