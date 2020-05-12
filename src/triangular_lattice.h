/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#ifndef LATTICE_TRIANGULAR_LATTICE_H
#define LATTICE_TRIANGULAR_LATTICE_H

#include "lattice.h"

namespace lattice {

class TriangularLattice : public Lattice {
public:
    explicit TriangularLattice(size_t size);

    size_t nodes_count() const override;

    size_t edges_count() const override;

    const std::vector<Node> &nodes() const override;

    std::vector<size_t> source_idx() const override;

    void drop_node(size_t node_id) override;

    void drop_edge_between(size_t n1, size_t n2) override;

private:
    size_t m_size;
    std::vector<Node> m_nodes;

    void create_nodes();
};

}

#endif //LATTICE_TRIANGULAR_LATTICE_H
