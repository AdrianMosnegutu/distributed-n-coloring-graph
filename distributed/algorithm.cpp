#include "algorithm.hpp"

#include <mpi.h>
#include <stdexcept>
#include <vector>

#ifndef DEBUG
#define DEBUG 0
#endif

namespace {

struct TurnResult {
  bool changed;
  bool failed;
};

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

TurnResult perform_coloring_turn(Graph &graph, const int world_rank, const int rank_turn,
                                 const int start_vertex, const int end_vertex, const int n_colors) {
  bool local_changed = false;
  bool local_failure = false;

  if (world_rank == rank_turn) {
    for (int i = start_vertex; i < end_vertex; ++i) {
      auto &node = graph.get_node(i);
      std::vector<bool> used_colors(n_colors + 1, false);

      for (const auto &neighbour_id : node.get_neighbours()) {
        const auto &neighbour = graph.get_node(neighbour_id);
        if (neighbour.get_color() != 0) {
          used_colors[neighbour.get_color()] = true;
        }
      }

      int new_color = 0;
      for (int c = 1; c <= n_colors; ++c) {
        if (!used_colors[c]) {
          new_color = c;
          break;
        }
      }

      if (new_color == 0 && node.get_color() == 0) {
        local_failure = true;
        break;
      }

      if (new_color != 0 && new_color != node.get_color()) {
        node.set_color(new_color);
        local_changed = true;
      }
    }
  }

  // First, perform a collective check for failure to prevent deadlock.
  bool global_failure;
  MPI_Allreduce(&local_failure, &global_failure, 1, MPI_CXX_BOOL, MPI_LOR, MPI_COMM_WORLD);

  if (global_failure) {
    return {false, true}; // A failure was detected, exit immediately.
  }

  // If no failure, proceed with the normal state synchronization.
  const size_t n_vertices = graph.get_vertex_count();
  auto colors = graph.get_colors();
  MPI_Bcast(colors.data(), n_vertices, MPI_INT, rank_turn, MPI_COMM_WORLD);

  if (world_rank != rank_turn) {
    graph.set_colors(colors);
  }

  bool global_change;
  MPI_Allreduce(&local_changed, &global_change, 1, MPI_CXX_BOOL, MPI_LOR, MPI_COMM_WORLD);

  return {global_change, false};
}

} // namespace

void run_parallel_coloring(Graph &graph, int world_size, int world_rank, int n_colors) {
  const size_t n_vertices = graph.get_vertex_count();

  std::vector<int> counts(world_size);
  std::vector<int> displs(world_size);
  distribute_vertices(n_vertices, world_size, counts, displs);

  const int start_vertex = displs[world_rank];
  const int end_vertex = start_vertex + counts[world_rank];

  bool changed;
  bool failure_detected = false;
  int iter = 0;

  do {
    if (DEBUG) {
      MPI_Barrier(MPI_COMM_WORLD);
      if (world_rank == 0) {
        printf("Iteration %d\n", iter++);
      }
    }

    bool global_changed_in_round = false;
    for (int rank_turn = 0; rank_turn < world_size; ++rank_turn) {
      const TurnResult result =
          perform_coloring_turn(graph, world_rank, rank_turn, start_vertex, end_vertex, n_colors);

      if (result.failed) {
        failure_detected = true;
        break;
      }
      if (result.changed) {
        global_changed_in_round = true;
      }
    }

    if (failure_detected) {
      break;
    }

    changed = global_changed_in_round;

  } while (changed);

  if (failure_detected) {
    throw std::runtime_error("Not enough colors available to color the graph.");
  }
}
