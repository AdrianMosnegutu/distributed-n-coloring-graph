#pragma once

#include <ostream>
#include <set>

/**
 * @brief Represents a node in an undirected graph.
 *
 * Each node has a unique ID, a color, and a set of neighbours.
 */
class Node {
public:
  /**
   * @brief Construct a new Node object
   *
   * @param value The node's ID.
   */
  explicit Node(int value);

  /**
   * @brief Get the ID of the node.
   *
   * @return const int The node's ID.
   */
  [[nodiscard]] const int get_id() const;

  /**
   * @brief Get the color of the node.
   *
   * @return const int The node's color.
   */
  [[nodiscard]] const int get_color() const;

  /**
   * @brief Get the set of neighbours of the node.
   *
   * @return const std::unordered_set<int>& A const reference to the node's set
   * of neighbours.
   */
  [[nodiscard]] const std::set<int> &get_neighbours() const;

  /**
   * @brief Set the color of the node to a new value.
   *
   * @param color The new color of the node.
   */
  void set_color(int color);

  /**
   * @brief Adds a neighbour to the node.
   *
   * @param node_id The ID of the neighbour to add.
   */
  void add_neighbour(int node_id);

  /**
   * @brief Overloads the << operator to allow pretty printing of the node.
   *
   * @param os The output stream.
   * @param node The node to print.
   * @return std::ostream& The output stream.
   */
  friend std::ostream &operator<<(std::ostream &os, const Node &node);

private:
  int id_;
  int color_;
  std::set<int> neighbours_;
};
