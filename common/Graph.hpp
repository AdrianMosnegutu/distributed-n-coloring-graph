#pragma once

#include "Node.hpp"
#include <ostream>
#include <vector>

/**
 * @brief Represents an undirected graph with a fixed number of vertices.
 *
 * This class stores a graph as a vector of Nodes. It provides methods for
 * creating a random graph, adding edges, and accessing nodes.
 */
class Graph {
public:
  /**
   * @brief Construct a new Graph object
   *
   * @param vertex_count The number of vertices in the graph
   */
  explicit Graph(int vertex_count);

  /**
   * @brief Generates a random graph with a given number of vertices and
   * density.
   *
   * @param vertex_count The number of vertices in the graph.
   * @param density The density of the graph, a value between 0 and 1.
   * @return Graph A new random graph.
   */
  static Graph random_graph(const int vertex_count, const double density);

  /**
   * @brief Get a reference to a node by its ID.
   *
   * @param id The ID of the node to get.
   * @return Node& A reference to the node.
   */
  Node &get_node(int id);

  /**
   * @brief Adds an undirected edge between two nodes.
   *
   * @param start The ID of the first node.
   * @param end The ID of the second node.
   */
  void add_edge(int start, int end);

  /**
   * @brief Overloads the << operator to allow pretty printing of the graph.
   *
   * @param os The output stream.
   * @param g The graph to print.
   * @return std::ostream& The output stream.
   */
  friend std::ostream &operator<<(std::ostream &os, const Graph &g);

private:
  std::vector<Node> nodes_;
};
