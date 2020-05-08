/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#include <algorithm>
#include <numeric>

#include "square_lattice.h"

namespace lattice {

SquareLattice::SquareLattice(std::size_t size) : m_size(size) {
    create_nodes();
}

void SquareLattice::create_nodes() {
    m_nodes.reserve(m_size * m_size);

    for (std::size_t i = 0; i < m_size; ++i) {
        for (std::size_t j = 0; j < m_size; ++j) {
            std::vector<size_t> adj;
            Node::Type type = Node::Type::INTERMEDIATE;

            // Bottom edge come first
            if (i < m_size - 1)
                adj.push_back((i + 1) * m_size + j);
            else
                type = Node::Type::TARGET;

            // Then the right one
            if (j < m_size - 1)
                adj.push_back(i * m_size + j + 1);
            // Then the left one
            if (j > 0)
                adj.push_back(i * m_size + j - 1);

            // Finally the top one
            if (i > 0)
                adj.push_back((i - 1) * m_size + j);
            else
                type = Node::Type::SOURCE;

            m_nodes.emplace_back(adj, type);
        }
    }
}

size_t SquareLattice::nodes_count() {
    return m_size * m_size;
}

size_t SquareLattice::edges_count() {
    return 2 * m_size * (m_size - 1);
}

const std::vector<Node> &SquareLattice::nodes() {
    return m_nodes;
}

std::vector<size_t> SquareLattice::source_idx() {
    std::vector<size_t> idx(m_size);
    std::iota(idx.begin(), idx.end(), m_size);
    return idx;
}

void SquareLattice::drop_node(size_t node_id) {
    Node &node = m_nodes.at(node_id);
    for (auto adj_node_id : node.nodes) {
        Node &adj_node = m_nodes.at(adj_node_id);
        size_t i;
        for (i = 0; i < adj_node.nodes.size(); ++i)
            if (adj_node.nodes[i] == node_id)
                break;
        adj_node.nodes.erase(adj_node.nodes.begin() + i);
    }
    node.nodes.clear();
    node.nodes.shrink_to_fit();
}

void SquareLattice::drop_edge_between(size_t n1, size_t n2) {
    Node &node1 = m_nodes.at(n1), node2 = m_nodes.at(n2);
    size_t i;

    for (i = 0; i < node1.nodes.size(); ++i)
        if (node1.nodes[i] == n2)
            break;
    node1.nodes.erase(node1.nodes.begin() + i);

    for (i = 0; i < node2.nodes.size(); ++i)
        if (node2.nodes[i] == n1)
            break;
    node2.nodes.erase(node2.nodes.begin() + i);
}

}
