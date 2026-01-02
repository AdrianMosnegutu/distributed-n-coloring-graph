#include "Graph.hpp"
#include <ostream>
#include <random>

Graph::Graph(int vertex_count) {
  nodes_.reserve(vertex_count);
  for (int i = 0; i < vertex_count; ++i) {
    nodes_.emplace_back(i);
  }
}

Graph Graph::random_graph(const int vertex_count, const double density) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0.0, 1.0);

  Graph g(vertex_count);

  for (int i = 0; i < vertex_count; ++i) {
    for (int j = i + 1; j < vertex_count; ++j) {
      if (dis(gen) < density) {
        g.add_edge(i, j);
      }
    }
  }

  return g;
}

Node &Graph::get_node(int node_id) { return nodes_[node_id]; }

void Graph::add_edge(int start, int end) {
  get_node(start).add_neighbour(end);
  get_node(end).add_neighbour(start);
}

std::ostream &operator<<(std::ostream &os, const Graph &g) {
  for (const auto &node : g.nodes_) {
    os << node << '\n';
  }

  return os;
}
