#include "../common/Graph.hpp"
#include <cstdlib>
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: " << argv[0]
              << " <vertex_count> <density> <num_colors>";
    return EXIT_FAILURE;
  }

  const int vertex_count = atoi(argv[1]);
  const double density = atof(argv[2]);
  const int num_colors = atoi(argv[3]);

  Graph g = Graph::random_graph(vertex_count, density);
  std::cout << g;

  return EXIT_SUCCESS;
}
