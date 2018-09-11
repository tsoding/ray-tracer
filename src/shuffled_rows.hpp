#ifndef SHUFFLED_ROWS_HPP_
#define SHUFFLED_ROWS_HPP_

#include <utility>
#include <vector>

template <typename RowRendering>
class ShuffledRows {
 public:
    explicit ShuffledRows(RowRendering &&rowRendering):
        m_rowRendering(std::move(rowRendering)),
        m_shuffledRows(m_rowRendering.height()) {
        std::iota(m_shuffledRows.begin(),
                  m_shuffledRows.end(),
                  0);
        std::random_shuffle(m_shuffledRows.begin(),
                            m_shuffledRows.end());
    }

    inline void renderRow(size_t row) {
        m_rowRendering.renderRow(m_shuffledRows[row]);
    }

    inline size_t height() const {
        return m_rowRendering.height();
    }

 private:
    RowRendering m_rowRendering;
    std::vector<size_t> m_shuffledRows;
};

template <typename RowRendering>
inline ShuffledRows<RowRendering>
mkShuffledRows(RowRendering &&rowRendering) {
    return ShuffledRows<RowRendering>(std::move(rowRendering));
}

#endif  // SHUFFLED_ROWS_HPP_
