/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#include <numeric>
#include "triangular_lattice.h"

namespace lattice {

TriangularLattice::TriangularLattice(size_t size) : m_size(size) {
    create_nodes();
}

void TriangularLattice::create_nodes() {
    m_nodes.reserve(m_size * m_size);
    m_edges.reserve((m_size - 1) * (3 * m_size - 1));

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
            std::vector<size_t> edges;
            Node::Type type = Node::Type::INTERMEDIATE;
            if (i == 0)
                type = Node::Type::SOURCE;
            else if (i == m_size - 1)
                type = Node::Type::TARGET;

            size_t node_id = i * m_size + j;

            // As rows start from 0, the first is even. Based on these variables we can make a formula to calculate
            // indexes of adjacent nodes the same way for all nodes.
            bool even = (i % 2 == 0), odd = (i % 2 == 1);

            // Down-Left
            if (i < m_size - 1 && (j > 0 || odd)) {
                Edge down_left = Edge{node_id, node_id + m_size - even};
                m_edges.push_back(down_left);
                edges.push_back(m_edges.size() - 1);
            }
            // Down-Right
            if (i < m_size - 1 && (j < m_size - 1 || even)) {
                Edge down_right = Edge{node_id, node_id + m_size + odd};
                m_edges.push_back(down_right);
                edges.push_back(m_edges.size() - 1);
            }
            // Left
            if (j > 0) {
                size_t left_node = node_id - 1;
                auto &left_node_edges = m_nodes[left_node].edges;

                for (size_t &edge_id : left_node_edges) {
                    if (m_edges[edge_id].node_b == node_id) {
                        edges.push_back(edge_id);
                        break;
                    }
                }
            }
            // Right
            if (j < m_size - 1) {
                Edge right = Edge{node_id, node_id + 1};
                m_edges.push_back(right);
                edges.push_back(m_edges.size() - 1);
            }
            // Up-Left
            if (i > 0 && (j > 0 || odd)) {
                size_t up_left_node = node_id - m_size - even;
                auto &up_left_node_edges = m_nodes[up_left_node].edges;

                for (size_t &edge_id : up_left_node_edges) {
                    if (m_edges[edge_id].node_b == node_id) {
                        edges.push_back(edge_id);
                        break;
                    }
                }
            }
            // Up-Right
            if (i > 0 && (j < m_size - 1 || even)) {
                size_t up_right_node = node_id - m_size + odd;
                auto &up_right_node_edges = m_nodes[up_right_node].edges;

                for (size_t &edge_id : up_right_node_edges) {
                    if (m_edges[edge_id].node_b == node_id) {
                        edges.push_back(edge_id);
                        break;
                    }
                }
            }

            m_nodes.emplace_back(edges, type);
        }
    }
}

const std::vector<Node> &TriangularLattice::nodes() const {
    return this->m_nodes;
}

const std::vector<Edge> &TriangularLattice::edges() const {
    return this->m_edges;
}

std::vector<size_t> TriangularLattice::source_idx() const {
    std::vector<size_t> idx(m_size);
    std::iota(idx.begin(), idx.end(), 0);
    return idx;
}

void TriangularLattice::drop_node(size_t node_id) {
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

void TriangularLattice::drop_edge_between(size_t node_a, size_t node_b) {
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