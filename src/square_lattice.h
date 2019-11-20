/* Copyright 2019, Sergey Popov (me@sergobot.me) */

#ifndef LATTICE_SQUARE_LATTICE_H
#define LATTICE_SQUARE_LATTICE_H

#include <cstdlib>
#include <random>

namespace lattice {
class SquareLattice {
public:
    explicit SquareLattice(size_t size, std::mt19937 &rng);

    ~SquareLattice();

    void drop(size_t position);

    bool permeable();

    double find_threshold();

    void print(const bool *visited);

private:
    const std::size_t m_size;
    bool *m_lattice;

    std::mt19937 &m_rng;
    std::uniform_int_distribution<std::mt19937::result_type> m_dist;

    bool path_exists(bool *visited, size_t position);
};
}

#endif //LATTICE_SQUARE_LATTICE_H
