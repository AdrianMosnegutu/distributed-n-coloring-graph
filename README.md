# N-Coloring of a Graph

## Summary

This project provides implementations for solving the n-coloring problem of a graph using both parallel and distributed algorithms. The goal is to assign a color to each vertex from a set of 'n' available colors, such that no two adjacent vertices share the same color.

## Implementations

The project includes two distinct implementations:

1.  **Distributed (MPI):** Located in the `distributed/` directory, this version uses C++ and the Message Passing Interface (MPI) to solve the n-coloring problem across multiple processes.

2.  **Parallel (C++ Threads):** Located in the `regular/` directory, this version uses standard C++ threads to find a valid n-coloring in a shared-memory parallel environment.

## Building the Project

To compile the project, simply run the `make` command from the root of the project directory:

```bash
make
```

This will create two executables: `distributed` inside the `build/distributed/` directory and `regular` inside the `build/regular/` directory.

To build a specific version, you can use:

- `make build_distributed`
- `make build_regular`

## Running the Algorithms

You can use the `make` command to run the algorithms with default arguments.

### Distributed (MPI)

To run the distributed version, you can use:

```bash
make run_distributed ARGS="<num_processes> <vertex_count> <density> <num_colors>"
```

-   **Arguments:**
    -   `<num_processes>`: The number of MPI processes to use.
    -   `<vertex_count>`: The number of vertices in the graph.
    -   `<density>`: The probability of an edge between any two vertices (a value between 0.0 and 1.0).
    -   `<num_colors>`: The number of colors ('n') available for coloring.

-   **Example:**
    ```bash
    make run_distributed ARGS="4 100 0.2 4"
    ```
    This command runs the algorithm on 4 processes to find a 4-coloring for a 100-vertex graph with a density of 20%.

### Parallel (C++ Threads)

To run the parallel version, you can use:

```bash
make run_regular ARGS="<vertex_count> <density> <num_colors>"
```

-   **Arguments:**
    -   `<vertex_count>`: The number of vertices in the graph.
    -   `<density>`: The probability of an edge between any two vertices (a value between 0.0 and 1.0).
    -   `<num_colors>`: The number of colors ('n') available for coloring.

-   **Example:**
    ```bash
    make run_regular ARGS="100 0.2 4"
    ```
    This command runs the algorithm to find a 4-coloring for a 100-vertex graph with a density of 20%.
