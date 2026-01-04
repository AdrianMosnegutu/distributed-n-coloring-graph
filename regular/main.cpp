#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

using namespace std;

// Graph structure (no globals → MPI-ready)
struct Graph {
    int n;                          // number of nodes
    int k;                          // number of colors
    vector<vector<int>> adj;        // adjacency matrix
};

// Shared state
atomic<bool> any_solved(false);
vector<int> final_solution;
mutex sol_mutex;

// Check if assigning `color` to `node` is valid
bool is_safe(int node, int color, const vector<int>& colors, const Graph& g) {
    for (int i = 0; i < node; i++) {          // check only previous nodes
        if (g.adj[node][i] && colors[i] == color)
            return false;
    }
    return true;
}

// Backtracking coloring
void backtrack(int node, vector<int> colors, const Graph& g) {
    if (any_solved) return;

    if (node == g.n) {
        lock_guard<mutex> lock(sol_mutex);
        if (!any_solved) {
            final_solution = colors;
            any_solved = true;
        }
        return;
    }

    for (int c = 1; c <= g.k; c++) {
        if (is_safe(node, c, colors, g)) {
            colors[node] = c;
            backtrack(node + 1, colors, g);
            colors[node] = 0;
            if (any_solved) return;
        }
    }
}

int main() {
    Graph g;
    g.n = 4;
    g.k = 3;
    g.adj = {
        {0, 1, 1, 1},
        {1, 0, 1, 0},
        {1, 1, 0, 1},
        {1, 0, 1, 0}
    };

    vector<thread> threads;

    // Parallelize first decision level
    for (int c = 1; c <= g.k; c++) {
        threads.emplace_back([&g, c]() {
            vector<int> colors(g.n, 0);
            colors[0] = c;
            backtrack(1, colors, g);
        });
    }

    for (auto& t : threads)
        t.join();

    if (any_solved) {
        cout << "Solution found:\n";
        for (int i = 0; i < g.n; i++)
            cout << "Node " << i << " -> Color " << final_solution[i] << "\n";
    } else {
        cout << "No solution exists.\n";
    }

    return 0;
}
