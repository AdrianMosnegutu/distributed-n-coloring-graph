# N-Coloring of a Graph

A compact implementation of greedy graph coloring provided in two variants: a distributed parallel implementation using MPI and Luby's algorithm, and a shared-memory multithreaded implementation using C++ threads. The project demonstrates practical trade-offs between parallelism and synchronization overhead when keeping a global graph state consistent across workers.

## Highlights
- distributed/ — parallel greedy coloring based on Luby's algorithm implemented with MPI.
- regular/ — greedy graph coloring using C++ threads (shared-memory).
- The distributed implementation uses MPI barriers to synchronize coloring rounds and broadcasts the global graph state to keep processes consistent and up to date.

## Modules

### distributed/
- Implements a parallel greedy coloring strategy influenced by Luby's algorithm.
- Processes progress in synchronized rounds. Each round is synchronized with `MPI_Barrier` and the global graph/state is broadcast using `MPI_Bcast` so every process works from the same up-to-date view.
- Designed for correctness and clarity of synchronization; this design incurs communication overhead for state distribution.

### regular/
- Implements a greedy coloring algorithm using C++ threads.
- Suitable for shared-memory execution where synchronization is done with standard threading primitives (mutexes, atomics) and no inter-process communication is necessary.

## Performance (observed)
- For 1,000 vertices with 0.5 edge density, the distributed algorithm completes in approximately 0.05 seconds.
- For 10,000 vertices with the same 0.5 edge density, the distributed algorithm completes in approximately 10 seconds.
- The marked increase in time is primarily due to the cost of sending synchronization data across processes and updating a large number of vertices each round (communication and global-state update overhead).

## Requirements
- C++17-compatible compiler
- MPI implementation (e.g., OpenMPI, MPICH) for the distributed module
- pthreads (usually available via the system toolchain) for the regular module
- CMake or a simple Makefile/toolchain of your choice for building

## Build & Run (example)
- Build (distributed):
  ```bash
  mpic++ -std=c++17 -O2 distributed/*.cpp -o distributed/ncolor
  mpirun -np <num_procs> ./distributed/ncolor <graph_input> [options]
  ```
- Build (regular):
  ```bash
  g++ -std=c++17 -O2 regular/*.cpp -pthread -o regular/ncolor
  ./regular/ncolor <graph_input> [options]
  ```

Adjust commands to match your environment or provided build scripts.

## Notes
- The distributed implementation favors clear, synchronous rounds to ensure consistent color choices across processes; this design intentionally trades communication overhead for straightforward correctness in a parallel setting.
- Use input sizes and process counts appropriate for your network and machine configuration; performance varies significantly with graph size, density, and number of processes.
