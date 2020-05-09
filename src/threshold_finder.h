/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#ifndef LATTICE_THRESHOLD_FINDER_H
#define LATTICE_THRESHOLD_FINDER_H

#include <random>
#include <deque>
#include <memory>
#include "lattice.h"

namespace lattice {

class ThresholdFinder {
public:
    enum Mode {
        EDGES, NODES
    };

    ThresholdFinder() = default;

    static double run(size_t iterations, size_t threads, Mode mode, std::unique_ptr<Lattice> (*generator)());

private:
    static double find_threshold(std::unique_ptr<Lattice> (*generator)(), size_t iterations, Mode mode);

    static bool is_permeable(const Lattice &lat);

    static bool path_exists(const Lattice &lat, const size_t &from, std::deque<bool> &visited);
};
}

#endif //LATTICE_THRESHOLD_FINDER_H
