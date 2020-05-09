/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#ifndef LATTICE_SQUARE_LATTICE_H
#define LATTICE_SQUARE_LATTICE_H

#include "lattice.h"

namespace lattice {

class SquareLattice : public Lattice {
public:
    explicit SquareLattice(std::size_t size);

    size_t nodes_count() const override;

    size_t edges_count() const override;

    const std::vector<Node> &nodes() const override;

    std::vector<size_t> source_idx() const override;

    void drop_node(size_t node) override;

    void drop_edge_between(size_t n1, size_t n2) override;

private:
    const std::size_t m_size;
    std::vector<Node> m_nodes;

    void create_nodes();
};

}

#endif //LATTICE_SQUARE_LATTICE_H
