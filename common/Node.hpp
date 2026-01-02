#pragma once

#include <ostream>
#include <set>

class Node {
public:
  explicit Node(int value);
  ~Node() = default;

  const int get_id() const;
  const int get_color() const;
  const std::set<int> &get_neighbours() const;

  void add_neighbour(int node_id);

  friend std::ostream &operator<<(std::ostream &os, const Node &node);

private:
  int id_;
  int color_;
  std::set<int> neighbours_;
};
