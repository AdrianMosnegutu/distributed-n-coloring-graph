#pragma once

#include "../common/Graph.hpp"

void run_parallel_coloring(Graph &graph, int world_size, int world_rank, int n_colors);
