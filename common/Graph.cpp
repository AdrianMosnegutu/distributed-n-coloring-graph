#include "Graph.hpp"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <random>
#include <vector>

Graph::Graph(int vertex_count) {
  nodes_.reserve(vertex_count);
  for (int i = 0; i < vertex_count; ++i) {
    nodes_.emplace_back(i);
  }
}

Graph Graph::random_graph(const int vertex_count, const double density, const int seed) {
  std::mt19937 gen(seed == -1 ? std::random_device()() : seed);
  std::uniform_real_distribution<> dis(0.0, 1.0);

  Graph g(vertex_count);
  const double clamped_density = std::clamp(density, 0.0, 1.0);

  for (int i = 0; i < vertex_count; ++i) {
    for (int j = i + 1; j < vertex_count; ++j) {
      if (dis(gen) < clamped_density) {
        g.add_edge(i, j);
      }
    }
  }

  return g;
}

std::vector<int> Graph::serialize() const {
  std::vector<int> edge_list;

  for (const auto &node : nodes_) {
    for (const auto &neighbour_id : node.get_neighbours()) {
      if (node.get_id() < neighbour_id) {
        edge_list.push_back(node.get_id());
        edge_list.push_back(neighbour_id);
      }
    }
  }

  return edge_list;
}

Graph Graph::deserialize(int vertex_count, const std::vector<int> &edge_list) {
  Graph g(vertex_count);

  for (size_t i = 0; i < edge_list.size(); i += 2) {
    g.add_edge(edge_list[i], edge_list[i + 1]);
  }

  return g;
}

Node &Graph::get_node(int node_id) { return nodes_[node_id]; }

const Node &Graph::get_node(int node_id) const { return nodes_[node_id]; }

size_t Graph::get_vertex_count() const { return nodes_.size(); }

std::vector<int> Graph::get_colors() const {
  std::vector<int> colors;

  colors.reserve(nodes_.size());
  for (const auto &node : nodes_) {
    colors.push_back(node.get_color());
  }

  return colors;
}

void Graph::set_colors(const std::vector<int> &colors) {
  for (size_t i = 0; i < colors.size(); ++i) {
    nodes_[i].set_color(colors[i]);
  }
}

void Graph::add_edge(int start, int end) {
  get_node(start).add_neighbour(end);
  get_node(end).add_neighbour(start);
}

bool Graph::validate_coloring() const {
  for (const auto &node : nodes_) {
    if (node.get_color() == 0) {
      std::cerr << "Error: Vertex " << node.get_id() << " is not colored.\n";
      return false;
    }

    for (const auto &neighbour_id : node.get_neighbours()) {
      if (node.get_color() == get_node(neighbour_id).get_color()) {
        std::cerr << "Error: Adjacent vertices ";
        std::cerr << node.get_id() << " and " << neighbour_id;
        std::cerr << " have the same color " << node.get_color() << '\n';
        return false;
      }
    }
  }

  return true;
}

std::ostream &operator<<(std::ostream &os, const Graph &g) {
  os << "graph {\n";

  for (const auto &node : g.nodes_) {
    os << node << '\n';
  }

  for (const auto &node : g.nodes_) {
    for (const auto &neighbour_id : node.get_neighbours()) {
      if (node.get_id() < neighbour_id) {
        os << "  " << node.get_id() << " -- " << neighbour_id << ";\n";
      }
    }
  }

  os << "}\n";
  return os;
}
