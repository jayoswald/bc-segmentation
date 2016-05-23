#include "graph.h"
#include <iostream>

std::vector<double> betweeness_centrality(const Graph &g) {

    const auto N = g.vertices.size();

    struct Paths {
        int paths[6];
        int count = 0;
        void add(int i) { 
            if (count == 6) {
                std::cerr << "Overflowed paths.\n";
            }
            paths[count++] = i; 
        }
    };

    std::vector<double> cb(N, 0.0);

    #pragma omp parallel for
    for (int k=0; k<g.vertices.size(); ++k) { 
        // Start at a vertex s.
        auto s = g.vertices[k];
        auto P = std::vector<Paths>(N);
        auto sigma = std::vector<double>(N,0.0);   sigma[s] = 1.0;
        auto dist  = std::vector<int>(N,-1);    dist[s] = 0;
        auto order = std::vector<int>(N,-1);

        int last = 0;
        order[last] = s;

        // Loop over all vertices.
        for (int i=0; i<=last; ++i) {
            auto v = g.vertices[order[i]];
            for (auto w: g.neighbors[v]) {
                // w is found for the first time?
                if (dist[w] == -1) {
                    dist[w] = dist[v] + 1;
                    order[++last] = w;
                }
                // shortest path to w via v?
                if (dist[w] == dist[v]+1) {
                    sigma[w] += sigma[v];
                    P[w].add(v);
                }
            }
        }
        auto delta = std::vector<double>(N, 0.0);
        for (int i=last; i>=0; i--) {
            auto w = order[i];
            for (int j=0; j<P[w].count; ++j) {
                int v = P[w].paths[j];
                delta[v] += sigma[v]/sigma[w]*(1.0+delta[w]);
            }
            if (w != s) {
                #pragma omp atomic 
                cb[w] += delta[w];
            }
        }
    }
    for (auto &x: cb) {
        x /= 2.0;
        //x *= 2.0 / ((N-1)*(N-2));
    }
    return cb;
}

Graph random_graph(int n) {
    Graph g;

    const int A = n*n;
    const int V = n*n*n;
    int total_neighbors = 0;

    g.neighbors.assign(V, {});

    for (int i=0; i<V; ++i) {
        g.vertices.push_back(i);

        int ix = i%n;
        int iy = (i/n)%n;
        int iz = i/A;

        if (ix > 0)   g.neighbors[i].push_back(i-1);
        if (ix+1 < n) g.neighbors[i].push_back(i+1);
        if (iy > 0)   g.neighbors[i].push_back(i-n);
        if (iy+1 < n) g.neighbors[i].push_back(i+n);
        if (iz > 0)   g.neighbors[i].push_back(i-A);
        if (iz+1 < n) g.neighbors[i].push_back(i+A);
        total_neighbors += g.neighbors[i].size();
    }
    std::cout << "Created graph with " << g.vertices.size() 
              << " nodes and " << total_neighbors/2 << " edges.\n";
    return g;
}

