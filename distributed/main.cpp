#include "../common/Graph.hpp"
#include "algorithm.hpp"
#include "config.hpp"
#include <fstream>
#include <iostream>
#include <mpi.h>
#include <unordered_set>
#include <vector>

Graph broadcast_graph() {
  Graph g = Graph::random_graph(VERTEX_COUNT, EDGE_DENSITY, SEED);

  auto edge_list = g.serialize();
  int edge_list_size = (int)edge_list.size();

  MPI_Bcast(&edge_list_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(edge_list.data(), edge_list_size, MPI_INT, 0, MPI_COMM_WORLD);

  return g;
}

Graph receive_graph() {
  int edge_list_size;
  MPI_Bcast(&edge_list_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

  std::vector<int> edge_list(edge_list_size);
  MPI_Bcast(edge_list.data(), edge_list_size, MPI_INT, 0, MPI_COMM_WORLD);

  return Graph::deserialize(VERTEX_COUNT, edge_list);
}

double run_and_benchmark_coloring(Graph &graph, const int world_size, const int world_rank) {
  MPI_Barrier(MPI_COMM_WORLD);
  double start_time = 0.0, end_time = 0.0;

  if (world_rank == 0) {
    start_time = MPI_Wtime();
  }

  run_parallel_coloring(graph, world_size, world_rank, COLORS);

  if (world_rank == 0) {
    end_time = MPI_Wtime();
    return end_time - start_time;
  }

  return 0.0;
}

void process_and_print_results(const double elapsed_time, const Graph &graph) {
  std::cout << "\nAlgorithm execution time: " << elapsed_time << " seconds\n";

  if (VERTEX_COUNT <= MAX_VERTICES_TO_PRINT) {
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
  } else {
    std::cout << "\nGraph is too large to print (>" << MAX_VERTICES_TO_PRINT << " vertices).\n";
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

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int world_size, world_rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  Graph graph = world_rank == 0 ? broadcast_graph() : receive_graph();
  const double elapsed_time = run_and_benchmark_coloring(graph, world_size, world_rank);

  if (world_rank == 0) {
    process_and_print_results(elapsed_time, graph);
  }

  MPI_Finalize();
  return 0;
}
