#include "../common/Graph.hpp"
#include "algorithm.hpp"
#include "config.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <stdexcept>
#include <unordered_set>

Graph broadcast_graph(const int vertex_count, const double density, const int seed = -1) {
  Graph g = Graph::random_graph(vertex_count, density, seed);
  auto edge_list = g.serialize();
  int edge_list_size = (int)edge_list.size();

  MPI_Bcast(&edge_list_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(edge_list.data(), edge_list_size, MPI_INT, 0, MPI_COMM_WORLD);

  return g;
}

Graph receive_graph(const int vertex_count) {
  int edge_list_size;
  MPI_Bcast(&edge_list_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

  std::vector<int> edge_list(edge_list_size);
  MPI_Bcast(edge_list.data(), edge_list_size, MPI_INT, 0, MPI_COMM_WORLD);

  return Graph::deserialize(vertex_count, edge_list);
}

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  Graph graph =
      world_rank == 0 ? broadcast_graph(VERTEX_COUNT, EDGE_DENSITY) : receive_graph(VERTEX_COUNT);

  try {
    run_parallel_coloring(graph, world_size, world_rank, COLORS);
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

    std::unordered_set<int> unique_colors;
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
