/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#include <numeric>
#include "triangular_lattice.h"

namespace lattice {

TriangularLattice::TriangularLattice(size_t size) : m_size(size) {
    create_nodes();
}

void TriangularLattice::create_nodes() {
    m_nodes.reserve(m_size * m_size);

    /*
     * This function creates the lattice by filling m_nodes in an intuitive way with nodes in the following structure:
     *   X - X - X - X
     *    \ / \ / \ / \
     *     X - X - X - X
     *    / \ / \ / \ /
     *   X - X - X - X
     *    \ / \ / \ / \
     *     X - X - X - X
     * Most of the nodes have 6 adjacent nodes, added in the following order (from 0 to 5):
     *     4 - 5
     *    / \ / \
     *   2 - X - 3
     *    \ / \ /
     *     0 - 1
     */

    for (size_t i = 0; i < m_size; ++i) {
        for (size_t j = 0; j < m_size; ++j) {
            std::vector<size_t> adj;
            Node::Type type = Node::Type::INTERMEDIATE;
            if (i == 0)
                type = Node::Type::SOURCE;
            else if (i == m_size - 1)
                type = Node::Type::TARGET;

            // As rows start from 0, the first is even. Based on these variables we can make a formula to calculate
            // indexes of adjacent nodes the same way for all nodes.
            bool even = (i % 2 == 0), odd = (i % 2 == 1);

            // Down-Left
            if (i < m_size - 1 && (j > 0 || odd)) {
                adj.push_back((i + 1) * m_size + j - even);
            }
            // Down-Right
            if (i < m_size - 1 && (j < m_size - 1 || even)) {
                adj.push_back((i + 1) * m_size + j + odd);
            }
            // Left
            if (j > 0) {
                adj.push_back(i * m_size + j - 1);
            }
            // Right
            if (j < m_size - 1) {
                adj.push_back(i * m_size + j + 1);
            }
            // Up-Left
            if (i > 0 && (j > 0 || odd)) {
                adj.push_back((i - 1) * m_size + j - even);
            }
            // Up-Right
            if (i > 0 && (j < m_size - 1 || even)) {
                adj.push_back((i - 1) * m_size + j + odd);
            }

            m_nodes.emplace_back(adj, type);
        }
    }
}

size_t TriangularLattice::nodes_count() const {
    return m_size * m_size;
}

size_t TriangularLattice::edges_count() const {
    return (m_size - 1) * (3 * m_size - 1);
}

const std::vector<Node> &TriangularLattice::nodes() const {
    return this->m_nodes;
}

std::vector<size_t> TriangularLattice::source_idx() const {
    std::vector<size_t> idx(m_size);
    std::iota(idx.begin(), idx.end(), 0);
    return idx;
}

void TriangularLattice::drop_node(size_t node_id) {
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

void TriangularLattice::drop_edge_between(size_t n1, size_t n2) {
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