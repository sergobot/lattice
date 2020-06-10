/* Copyright 2020, Sergey Popov (me@sergobot.me) */
#ifndef LATTICE_EDGE_H
#define LATTICE_EDGE_H

#include <cstddef>

namespace lattice {

struct Edge {
    const size_t node_a;
    const size_t node_b;
};

}

#endif //LATTICE_EDGE_H
