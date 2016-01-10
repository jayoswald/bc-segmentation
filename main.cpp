#include <vector>
#include <iostream>
#include <queue>
#include <fstream>
#include "timer.h"

struct Graph {
    std::vector<int> vertices;
    std::vector<std::vector<int>> neighbors;
};

Graph random_graph(int n);
std::vector<double> brandes(const Graph &g);


int main() {
    auto graph = random_graph(15);

    auto timer = Timer();
    auto cb = brandes(graph);
    std::cout << "Computed centraility in " << timer.elapsed() << " seconds\n";

    std::fstream fid("result", std::ios::out);
    for (auto x: cb) {
        fid << x << "\n";
    }
}

Graph random_graph(int n) {
    Graph g;

    int a = n*n;
    int v = n*n*n;
    
    for (int i=0; i<v; ++i) {
        g.vertices.push_back(i);
        g.neighbors.emplace_back();

        int ix = i%n;
        int iy = (i/n)%n;
        int iz = i/a;

        if (ix-1 > 0) g.neighbors[i].push_back(i-1);
        if (ix+1 < n) g.neighbors[i].push_back(i+1);

        if (iy-1 > 0) g.neighbors[i].push_back(i-n);
        if (iy+1 < n) g.neighbors[i].push_back(i+n);

        if (iz-1 > 0) g.neighbors[i].push_back(i-a);
        if (iz+1 < n) g.neighbors[i].push_back(i+a);
    }
    std::cout << "Created graph with " << g.vertices.size() 
              << " nodes and " << g.neighbors.size()/2 << " edges.\n";
    return g;
}

std::vector<double> brandes(const Graph &g) {

    const auto N = g.vertices.size();
    std::vector<double> cb(N, 0.0);

    for (auto s: g.vertices) {
        auto P = std::vector<std::vector<int>>(N);
        auto sigma = std::vector<double>(N);   sigma[s] = 1.0;
        auto dist = std::vector<int>(N,-1);    dist[s] = 0;
        std::queue<int> queue;
        queue.push(s);

        // stack stores nodes with increasing distrance from node s.
        auto stack = std::vector<int>{};

        while (!queue.empty()) {
            auto v = queue.front(); queue.pop();
            stack.push_back(v);
            for (auto w: g.neighbors[v]) {
                
                // w is found for the first time?
                if (dist[w] < 0) {
                    queue.push(w);
                    dist[w] = dist[v] + 1;
                }
                // shortest path to w via v?
                if (dist[w] == dist[v]+1) {
                    sigma[w] += sigma[v];
                    P[w].push_back(v);
                }
            }
        }
        auto delta = std::vector<double>(N, 0.0);
        for (auto it=stack.rbegin(); it!=stack.rend(); ++it) {
            auto w = *it;
            for (auto v: P[w]) {
                delta[v] += sigma[v]/sigma[w]*(1.0+delta[w]);
            }
            if (w != s) {
                cb[w] += delta[w];
            }
        }
    }
    for (auto &x: cb) {
        x *= 2.0 / ((N-1)*(N-2));
    }
    return cb;
}
