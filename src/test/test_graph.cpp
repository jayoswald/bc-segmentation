#include "catch.hpp"
#include "graph.h"

TEST_CASE("centrality on wheel graph", "[centrality]") {
    Graph g;
    int n = 7;
    g.neighbors.assign(n, {});
    g.vertices.push_back(0);
    for (int i=1; i<n; ++i) {
        g.vertices.push_back(i);
        g.neighbors[0].push_back(i);
        g.neighbors[i].push_back(0);

        if (i < n-1) {
            g.neighbors[i].push_back(i+1);
            g.neighbors[i+1].push_back(i);
        }
    }
    g.neighbors[1].push_back(n-1);
    g.neighbors[n-1].push_back(1);
    auto b = betweeness_centrality(g);
    REQUIRE( b[0] == Approx(0.5*(n-1)*(n-5)) );
    for (int i=1; i<n; ++i) {
        REQUIRE ( b[i] == Approx(0.5) );
    }
}
TEST_CASE("centrality on a crown graph", "[centrality]") {
    Graph g;
    g.vertices = {0,1,2,3,4,5,6,7};
    g.neighbors = {{5,6,7}, {4,6,7}, {4,5,7}, {4,5,6},
                   {1,2,3}, {0,2,3}, {0,1,3}, {0,1,2}};
    auto b = betweeness_centrality(g);
    for (int i=0; i<8; ++i) REQUIRE(b[i] == Approx(2.5));
}
