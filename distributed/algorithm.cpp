#include "algorithm.hpp"

#include <cstdlib>
#include <iostream>
#include <mpi.h>
#include <random>
#include <vector>

void distribute_vertices(const size_t n_vertices, const int world_size, std::vector<int> &counts,
                         std::vector<int> &displs) {
  const int vertices_per_process_base = n_vertices / world_size;
  for (int i = 0; i < world_size; ++i) {
    const int start = i * vertices_per_process_base;
    const int end = (i == world_size - 1) ? n_vertices : start + vertices_per_process_base;
    counts[i] = end - start;
    displs[i] = start;
  }
}

void run_parallel_coloring(Graph &graph, int world_size, int world_rank, int n_colors) {
  const size_t n_vertices = graph.get_vertex_count();

  // 1. Determine which vertices this process owns
  std::vector<int> counts(world_size);
  std::vector<int> displs(world_size);
  distribute_vertices(n_vertices, world_size, counts, displs);
  const int start_vertex = displs[world_rank];
  const int end_vertex = start_vertex + counts[world_rank];

  int uncolored_node_count = n_vertices;
  // Seed each generator with a combination of rank and time for better randomness
  std::mt19937 gen(world_rank + time(nullptr));

  // Main loop: continues until all nodes are colored
  while (uncolored_node_count > 0) {
    auto current_colors = graph.get_colors();

    // 2. Generate random values for local uncolored vertices
    std::vector<double> local_random_values(counts[world_rank]);
    for (int i = 0; i < counts[world_rank]; ++i) {
      const int vertex_idx = start_vertex + i;
      if (current_colors[vertex_idx] == 0) {
        local_random_values[i] = std::uniform_real_distribution<>(0.0, 1.0)(gen);
      } else {
        local_random_values[i] = -1.0; // Mark as not participating
      }
    }

    // 3. Share random values with all processes
    std::vector<double> all_random_values(n_vertices);
    MPI_Allgatherv(local_random_values.data(), counts[world_rank], MPI_DOUBLE,
                   all_random_values.data(), counts.data(), displs.data(), MPI_DOUBLE,
                   MPI_COMM_WORLD);

    // 4. Identify local "winners" (Maximal Independent Set)
    std::vector<int> local_winners;
    for (int i = start_vertex; i < end_vertex; ++i) {
      if (current_colors[i] == 0) { // Is it an uncolored candidate?
        bool is_winner = true;
        const double my_rand = all_random_values[i];
        for (const auto &neighbor_id : graph.get_node(i).get_neighbours()) {
          if (current_colors[neighbor_id] == 0) { // Is neighbor also a candidate?
            const double neighbor_rand = all_random_values[neighbor_id];
            if (neighbor_rand > my_rand) {
              is_winner = false;
              break;
            }
            // Tie-breaking rule for equal random numbers
            if (neighbor_rand == my_rand && neighbor_id > i) {
              is_winner = false;
              break;
            }
          }
        }
        if (is_winner) {
          local_winners.push_back(i);
        }
      }
    }

    // 5. Color local winners
    for (const auto &winner_id : local_winners) {
      std::vector<bool> used_colors(n_colors + 1, false);
      for (const auto &neighbor_id : graph.get_node(winner_id).get_neighbours()) {
        if (current_colors[neighbor_id] != 0) {
          used_colors[current_colors[neighbor_id]] = true;
        }
      }
      int new_color = 0;
      for (int c = 1; c <= n_colors; ++c) {
        if (!used_colors[c]) {
          new_color = c;
          break;
        }
      }
      if (new_color == 0) {
        if (world_rank == 0) {
          std::cerr << "Error: Not enough colors available to color the graph. "
                       "Aborting.\n";
        }
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
      }
      graph.get_node(winner_id).set_color(new_color);
    }

    // 6. Share newly decided colors with all processes
    std::vector<int> local_new_colors(counts[world_rank]);
    for (int i = 0; i < counts[world_rank]; ++i) {
      local_new_colors[i] = graph.get_node(start_vertex + i).get_color();
    }

    std::vector<int> all_new_colors(n_vertices);
    MPI_Allgatherv(local_new_colors.data(), counts[world_rank], MPI_INT, all_new_colors.data(),
                   counts.data(), displs.data(), MPI_INT, MPI_COMM_WORLD);
    graph.set_colors(all_new_colors);

    // 7. Check for termination
    int nodes_colored_this_round;
    int local_winners_count = local_winners.size();
    MPI_Allreduce(&local_winners_count, &nodes_colored_this_round, 1, MPI_INT, MPI_SUM,
                  MPI_COMM_WORLD);

    if (uncolored_node_count > 0 && nodes_colored_this_round == 0) {
      if (world_rank == 0) {
        std::cerr << "Error: Algorithm stalled, no progress was made in a round. Aborting.\n";
      }

      MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    uncolored_node_count -= nodes_colored_this_round;
  }
}
