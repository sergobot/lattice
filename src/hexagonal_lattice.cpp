/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#include "hexagonal_lattice.h"
#include <numeric>

namespace lattice {

HexagonalLattice::HexagonalLattice(size_t size) : m_size(size), m_edges_count(0) {
    create_nodes();
}

void HexagonalLattice::create_nodes() {
    m_nodes.reserve(m_size * m_size);

    for (size_t i = 0; i < m_size; ++i) {
        for (size_t j = 0; j < m_size; ++j) {
            std::vector<size_t> adj;
            Node::Type type = Node::Type::INTERMEDIATE;
            if (i == 0)
                type = Node::Type::SOURCE;
            else if (i == m_size - 1)
                type = Node::Type::TARGET;

            // -1 is to the left, 1 is to the right
            short same_row_adj = (i % 2) ^ (j % 2) ? -1 : 1;

            // The only down edge for any node except the last row
            if (i < m_size - 1)
                adj.push_back((i + 1) * m_size + j);

            // The only horizontal edge for any node except half of the leftmost and rightmost ones
            if (!(j == 0 && same_row_adj == -1) && !(j == m_size - 1 && same_row_adj == 1))
                adj.push_back(i * m_size + j + same_row_adj);

            // The only up edge for any node except the first row
            if (i > 0)
                adj.push_back((i - 1) * m_size + j);

            m_edges_count += adj.size();
            m_nodes.emplace_back(adj, type);
        }
    }

    m_edges_count /= 2;
}

size_t HexagonalLattice::nodes_count() const {
    return m_size * m_size;
}

size_t HexagonalLattice::edges_count() const {
    return m_edges_count;
}

const std::vector<Node> &HexagonalLattice::nodes() const {
    return this->m_nodes;
}

std::vector<size_t> HexagonalLattice::source_idx() const {
    std::vector<size_t> idx(m_size);
    std::iota(idx.begin(), idx.end(), 0);
    return idx;
}

void HexagonalLattice::drop_node(size_t node_id) {
    Node &node = m_nodes[node_id];
    for (auto adj_node_id : node.nodes) {
        Node &adj_node = m_nodes[adj_node_id];
        size_t i;
        for (i = 0; i < adj_node.nodes.size(); ++i)
            if (adj_node.nodes[i] == node_id)
                break;
        if (i < adj_node.nodes.size())
            adj_node.nodes.erase(adj_node.nodes.begin() + i);
    }
    node.nodes.clear();
    node.nodes.shrink_to_fit();

}

void HexagonalLattice::drop_edge_between(size_t n1, size_t n2) {
    Node &node1 = m_nodes[n1], &node2 = m_nodes[n2];
    size_t i;

    for (i = 0; i < node1.nodes.size(); ++i)
        if (node1.nodes[i] == n2)
            break;
    if (i < node1.nodes.size())
        node1.nodes.erase(node1.nodes.begin() + i);

    for (i = 0; i < node2.nodes.size(); ++i)
        if (node2.nodes[i] == n1)
            break;
    if (i < node2.nodes.size())
        node2.nodes.erase(node2.nodes.begin() + i);
}



}