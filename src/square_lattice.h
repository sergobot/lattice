/* Copyright 2019, Sergey Popov (me@sergobot.me) */

#ifndef LATTICE_SQUARE_LATTICE_H
#define LATTICE_SQUARE_LATTICE_H

#include <cstdlib>
#include <random>

namespace lattice {
class SquareLattice {
public:
    enum EdgeOrientation {
        Horizontal = 0, Vertical = 1
    };

    explicit SquareLattice(size_t size, std::mt19937 &rng);

    ~SquareLattice();

    void drop_edge(EdgeOrientation orientation, size_t id);

    bool permeable();

    double find_threshold();

private:
    const std::size_t m_size;
    bool *m_horizontal_edges;
    bool *m_vertical_edges;

    std::mt19937 &m_rng;
    std::uniform_int_distribution<std::mt19937::result_type> m_dist;

    bool path_exists(bool *horizontal_visited, bool *vertical_visited, size_t x, size_t y);

    size_t down_edge(size_t x, size_t y);

    size_t right_edge(size_t x, size_t y);

    void print(const bool *horizontal_visited, const bool *vertical_visited);
};
}

#endif //LATTICE_SQUARE_LATTICE_H
