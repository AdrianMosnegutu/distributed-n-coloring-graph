#include "Node.hpp"

Node::Node(int id) : id_(id), color_(-1) {}

const int Node::get_id() const { return id_; }

const int Node::get_color() const { return color_; }

const std::set<int> &Node::get_neighbours() const { return neighbours_; }

void Node::set_color(int color) { color_ = color; }

void Node::add_neighbour(int node_id) { neighbours_.insert(node_id); }

std::ostream &operator<<(std::ostream &os, const Node &node) {
  os << "Node " << node.get_id() << " neighbours: ";
  for (const auto &neighbour_id : node.neighbours_) {
    os << neighbour_id << " ";
  }

  return os;
}
