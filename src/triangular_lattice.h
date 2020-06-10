/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#ifndef LATTICE_TRIANGULAR_LATTICE_H
#define LATTICE_TRIANGULAR_LATTICE_H

#include "lattice.h"

namespace lattice {

class TriangularLattice : public Lattice {
public:
    explicit TriangularLattice(size_t size);

    const std::vector<Edge> &edges() const override;

    const std::vector<Node> &nodes() const override;

    std::vector<size_t> source_idx() const override;

    void drop_node(size_t node_id) override;

    void drop_edge_between(size_t node_a, size_t node_b) override;

private:
    size_t m_size;
    std::vector<Node> m_nodes;
    std::vector<Edge> m_edges;

    void create_nodes();
};

}

#endif //LATTICE_TRIANGULAR_LATTICE_H
