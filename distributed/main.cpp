#include "../common/Graph.hpp"
#include "algorithm.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <set>
#include <stdexcept>

constexpr int N_COLORS = 10;

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  Graph graph = Graph::random(20, 0.15, 42);

  // Graph graph(10);
  // graph.add_edge(0, 1);
  // graph.add_edge(0, 2);
  // graph.add_edge(1, 2);
  // graph.add_edge(1, 3);
  // graph.add_edge(2, 3);
  // graph.add_edge(2, 4);
  // graph.add_edge(3, 4);
  // graph.add_edge(3, 5);
  // graph.add_edge(4, 5);
  // graph.add_edge(4, 6);
  // graph.add_edge(5, 6);
  // graph.add_edge(5, 7);
  // graph.add_edge(6, 7);
  // graph.add_edge(6, 8);
  // graph.add_edge(7, 8);
  // graph.add_edge(7, 9);
  // graph.add_edge(8, 9);

  try {
    run_parallel_coloring(graph, world_size, world_rank, N_COLORS);
  } catch (const std::runtime_error &e) {
    if (world_rank == 0) {
      std::cerr << "Error: " << e.what() << '\n';
    }

    MPI_Finalize();
    return EXIT_FAILURE;
  }

  if (world_rank == 0) {
    std::cout << "\nFinal graph coloring (dot format):\n";
    std::cout << graph;

    std::ofstream outfile("graph.out");
    if (outfile.is_open()) {
      outfile << graph;
      outfile.close();
      std::cout << "\nGraph also written to graph.out\n";
    } else {
      std::cerr << "Error: Unable to open graph.out for writing.\n";
    }

    std::set<int> unique_colors;
    for (const auto color : graph.get_colors()) {
      if (color != 0) {
        unique_colors.insert(color);
      }
    }
    std::cout << "\nNumber of colors used: " << unique_colors.size() << '\n';

    std::cout << "\nColoring is ";
    std::cout << (graph.validate_coloring() ? "valid" : "invalid");
    std::cout << ".\n";
  }

  MPI_Finalize();
  return 0;
}
