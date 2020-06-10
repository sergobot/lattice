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
    m_edges.reserve(2 * m_size * (m_size - 1));

    for (std::size_t i = 0; i < m_size; ++i) {
        for (std::size_t j = 0; j < m_size; ++j) {
            std::vector<size_t> edges;
            Node::Type type = Node::Type::INTERMEDIATE;
            size_t node_id = i * m_size + j;

            // Bottom edge comes first
            if (i < m_size - 1) {
                Edge bottom = {node_id, node_id + m_size};
                m_edges.push_back(bottom);
                edges.push_back(m_edges.size() - 1);
            } else {
                type = Node::Type::TARGET;
            }

            // Then the right one
            if (j < m_size - 1) {
                Edge right = {node_id, node_id + 1};
                m_edges.push_back(right);
                edges.push_back(m_edges.size() - 1);
            }

            // Then the left one
            if (j > 0) {
                size_t another_node = node_id - 1;
                auto &another_node_edges = m_nodes[another_node].edges;

                for (size_t &left : another_node_edges) {
                    if (m_edges[left].node_b == node_id) {
                        edges.push_back(left);
                        break;
                    }
                }
            }

            // Finally the top one
            if (i > 0) {
                size_t another_node = node_id - m_size;
                auto &another_node_edges = m_nodes[another_node].edges;

                for (size_t &top : another_node_edges) {
                    if (m_edges[top].node_b == node_id) {
                        edges.push_back(top);
                        break;
                    }
                }
            } else {
                type = Node::Type::SOURCE;
            }

            m_nodes.emplace_back(edges, type);
        }
    }
}

const std::vector<Edge> &SquareLattice::edges() const {
    return m_edges;
}

const std::vector<Node> &SquareLattice::nodes() const {
    return m_nodes;
}

std::vector<size_t> SquareLattice::source_idx() const {
    std::vector<size_t> idx(m_size);
    std::iota(idx.begin(), idx.end(), 0);
    return idx;
}

void SquareLattice::drop_node(size_t node_id) {
    Node &node = m_nodes[node_id];

    for (auto &edge : node.edges) {
        size_t adj_node_id = m_edges[edge].node_b == node_id ? m_edges[edge].node_b : m_edges[edge].node_a;

        Node &adj_node = m_nodes[adj_node_id];
        size_t i;
        for (i = 0; i < adj_node.edges.size(); ++i)
            if (adj_node.edges[i] == edge)
                break;
        if (i < adj_node.edges.size())
            adj_node.edges.erase(adj_node.edges.begin() + i);
    }
    node.edges.clear();
    node.edges.shrink_to_fit();
}

void SquareLattice::drop_edge_between(size_t node_a, size_t node_b) {
    Node &node1 = m_nodes[node_a], &node2 = m_nodes[node_b];
    size_t i;

    for (i = 0; i < node1.edges.size(); ++i)
        if (m_edges[node1.edges[i]].node_a == node_b || m_edges[node1.edges[i]].node_b == node_b)
            break;
    if (i < node1.edges.size())
        node1.edges.erase(node1.edges.begin() + i);

    for (i = 0; i < node2.edges.size(); ++i)
        if (m_edges[node2.edges[i]].node_a == node_a || m_edges[node2.edges[i]].node_b == node_a)
            break;
    if (i < node2.edges.size())
        node2.edges.erase(node2.edges.begin() + i);
}

}
