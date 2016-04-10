#pragma once
#include <vector>

struct Graph {
    std::vector<int> vertices;
    std::vector<std::vector<int>> neighbors;
};

//! Computes betweenness centrality using Brandes' algorithm.
std::vector<double> betweeness_centrality(const Graph &g);

//! Constructs a random graph.
Graph random_graph(int n);
