/* Copyright 2019, Sergey Popov (me@sergobot.me) */

#ifndef LATTICE_SQUARE_LATTICE_H
#define LATTICE_SQUARE_LATTICE_H

#include <cstdlib>
#include <random>

namespace lattice {
class SquareLattice {
public:
    explicit SquareLattice(size_t size, std::mt19937 & rng);

    ~SquareLattice();

    double find_threshold();

private:
    std::size_t size;
    bool *lattice;

    std::mt19937 & rng;
    std::uniform_int_distribution<std::mt19937::result_type> dist;

    void drop(size_t position);

    bool path_exists(bool *visited, size_t position);

    bool permeable();

    void print_lattice(bool *lattice);
};
}

#endif //LATTICE_SQUARE_LATTICE_H
