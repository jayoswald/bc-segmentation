#pragma once
#include <string>
#include "graph.h"

struct Voxel {
    int i, j, k;
    Voxel neighbor(int n) {
        switch (n) {
            case 0: return  {i-1, j, k};
            case 1: return  {i+1, j, k};
            case 2: return  {i, j-1, k};
            case 3: return  {i, j+1, k};
            case 4: return  {i, j, k-1};
            default: return {i, j, k+1};
        };
    };
};

//! Reads a set of voxels from an input file.
std::vector<Voxel> read_voxelset(std::string path);
//! Builds a graph from a set of voxels.
Graph voxelset_to_graph(std::vector<Voxel> &voxels);

inline bool operator<(const Voxel &a, const Voxel &b) {
    if (a.k < b.k) return true;
    if (a.k > b.k) return false;
    if (a.j < b.j) return true;
    if (a.j > b.j) return false;
    return a.i < b.i;
};

