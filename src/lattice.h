/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#ifndef LATTICE_LATTICE_H
#define LATTICE_LATTICE_H

#include "node.h"

namespace lattice {

class Lattice {
public:
    virtual ~Lattice() = default;

    virtual size_t nodes_count() const = 0;

    virtual size_t edges_count() const = 0;

    virtual const std::vector<Node> &nodes() const = 0;

    virtual std::vector<size_t> source_idx() const = 0;

    virtual void drop_node(size_t node) = 0;

    virtual void drop_edge_between(size_t node1, size_t node2) = 0;
};

    }
#endif //LATTICE_LATTICE_H
