/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#ifndef LATTICE_THRESHOLD_FINDER_H
#define LATTICE_THRESHOLD_FINDER_H

#import <random>
#import <deque>
#import "lattice.h"

namespace lattice {

class ThresholdFinder {
public:
    enum Mode {EDGES, NODES};

    ThresholdFinder() = default;

    static double run(size_t iterations, size_t threads, Mode mode, Lattice& (*generator)());
private:
    static double find_threshold(Lattice& (*generator)(), size_t iterations, Mode mode);
    static bool is_permeable(Lattice &lat);
    static bool path_exists(Lattice &lat, size_t &from, std::deque<bool> &visited);
};
}

#endif //LATTICE_THRESHOLD_FINDER_H
