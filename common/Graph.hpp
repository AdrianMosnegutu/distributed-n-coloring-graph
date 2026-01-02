#pragma once

#include "Node.hpp"
#include <ostream>
#include <vector>

class Graph {
public:
  explicit Graph(int vertex_count);
  ~Graph() = default;

  static Graph random_graph(const int vertex_count, const double density);

  Node &get_node(int id);
  void add_edge(int start, int end);

  friend std::ostream &operator<<(std::ostream &os, const Graph &g);

private:
  std::vector<Node> nodes_;
};
