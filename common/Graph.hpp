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
   * @brief Get a const reference to a node by its ID.
   *
   * @param id The ID of the node to get.
   * @return const Node& A const reference to the node.
   */
  [[nodiscard]] const Node &get_node(int id) const;

  /**
   * @brief Get the number of vertices in the graph.
   *
   * @return  The number of vertices.
   */
  [[nodiscard]] int get_vertex_count() const;

  /**
   * @brief Get the colors of all nodes in the graph.
   *
   * @return std::vector<int> A vector containing the color of each node.
   */
  [[nodiscard]] std::vector<int> get_colors() const;

  /**
   * @brief Set the colors of all nodes in the graph.
   *
   * @param colors A vector containing the new colors for the nodes.
   */
  void set_colors(const std::vector<int> &colors);

  /**
   * @brief Adds an undirected edge between two nodes.
   *
   * @param start The ID of the first node.
   * @param end The ID of the second node.
   */
  void add_edge(int start, int end);

  /**
   * @brief Validates the coloring of the graph.
   *
   * @return true if the coloring is valid, false otherwise.
   */
  [[nodiscard]] bool validate_coloring() const;

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
