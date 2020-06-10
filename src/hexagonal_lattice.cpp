/* Copyright 2020, Sergey Popov (me@sergobot.me) */

#include "hexagonal_lattice.h"
#include <numeric>

namespace lattice {

HexagonalLattice::HexagonalLattice(size_t size) : m_size(size) {
    create_nodes();
}

void HexagonalLattice::create_nodes() {
    m_nodes.reserve(m_size * m_size);
    m_edges.reserve(m_size * m_size); // This is not final, as the vector will probably grow.

    for (size_t i = 0; i < m_size; ++i) {
        for (size_t j = 0; j < m_size; ++j) {
            std::vector<size_t> edges;
            Node::Type type = Node::Type::INTERMEDIATE;
            size_t node_id = i * m_size + j;

            if (i == 0)
                type = Node::Type::SOURCE;
            else if (i == m_size - 1)
                type = Node::Type::TARGET;

            // -1 is to the left, 1 is to the right
            short same_row_adj = (i % 2) ^ (j % 2) ? -1 : 1;

            // The only down edge for any node except the last row
            if (i < m_size - 1) {
                Edge down = {node_id, node_id + m_size};
                m_edges.push_back(down);
                edges.push_back(m_edges.size() - 1);
            }

            // The only horizontal edge for any node except half of the leftmost and rightmost ones
            if (!(j == 0 && same_row_adj == -1) && !(j == m_size - 1 && same_row_adj == 1)) {
                if (same_row_adj == -1) {
                    size_t another_node = node_id - 1;
                    auto &another_node_edges = m_nodes[another_node].edges;

                    for (size_t &left : another_node_edges) {
                        if (m_edges[left].node_b == node_id) {
                            edges.push_back(left);
                            break;
                        }
                    }
                } else {
                    Edge right = {node_id, node_id + 1};
                    m_edges.push_back(right);
                    edges.push_back(m_edges.size() - 1);
                }
            }

            // The only up edge for any node except the first row
            if (i > 0) {
                size_t up_node = node_id - m_size;
                auto &up_node_edges = m_nodes[up_node].edges;

                for (size_t &edge : up_node_edges) {
                    if (m_edges[edge].node_b == node_id) {
                        edges.push_back(edge);
                        break;
                    }
                }
            }

            m_nodes.emplace_back(edges, type);
        }
    }
}

const std::vector<Edge> &HexagonalLattice::edges() const {
    return this->m_edges;
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

void HexagonalLattice::drop_edge_between(size_t n1, size_t n2) {
    Node &node1 = m_nodes[n1], &node2 = m_nodes[n2];
    size_t i;

    for (i = 0; i < node1.edges.size(); ++i)
        if (m_edges[node1.edges[i]].node_a == n2 || m_edges[node1.edges[i]].node_b == n2)
            break;
    if (i < node1.edges.size())
        node1.edges.erase(node1.edges.begin() + i);

    for (i = 0; i < node2.edges.size(); ++i)
        if (m_edges[node2.edges[i]].node_a == n1 || m_edges[node2.edges[i]].node_b == n1)
            break;
    if (i < node2.edges.size())
        node2.edges.erase(node2.edges.begin() + i);
}

}