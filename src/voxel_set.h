#pragma once
#include <string>
#include "graph.h"

struct Voxel {
    int i, j, k;
    Voxel neighbor(int n) const;
};

class VoxelSet {
public:
    size_t index(const Voxel &v) const;
    std::vector<Voxel> voxels;
    int xlo, xhi;
    int ylo, yhi;
    int zlo, zhi;
};

//! Reads a set of voxels from an input file.
VoxelSet read_voxelset(std::string path);
//! Reads a set of voxels from an input file containing multiple clusters.
VoxelSet read_voxelset(std::string path, int i);
//! Builds a graph from a set of voxels.
Graph voxelset_to_graph(const VoxelSet &voxels);

inline bool operator<(const Voxel &a, const Voxel &b) {
    if (a.k < b.k) return true;
    if (a.k > b.k) return false;
    if (a.j < b.j) return true;
    if (a.j > b.j) return false;
    return a.i < b.i;
};

inline bool operator==(const Voxel &a, const Voxel &b) {
    return (a.i == b.i) && (a.j == b.j) && (a.k == b.k);
}

inline bool operator!=(const Voxel &a, const Voxel &b) {
    return !(a == b);
}

inline Voxel Voxel::neighbor(int n) const {
    switch (n) {
    // Nearest neighbors (6).
    case 0: return {i-1, j, k};
    case 1: return {i+1, j, k};
    case 2: return {i, j-1, k};
    case 3: return {i, j+1, k};
    case 4: return {i, j, k-1};
    case 5: return {i, j, k+1};
    // Second nearest neighbors (12)
    case 6: return {i-1, j-1, k};
    case 7: return {i+1, j-1, k};
    case 8: return {i+1, j+1, k};
    case 9: return {i-1, j+1, k};
    case 10: return {i-1, j, k-1};
    case 11: return {i+1, j, k-1};
    case 12: return {i+1, j, k+1};
    case 13: return {i-1, j, k+1};
    case 14: return {i, j-1, k-1};
    case 15: return {i, j+1, k-1};
    case 16: return {i, j+1, k+1};
    case 17: return {i, j-1, k+1};
    // Third nearest neighbors (8)
    case 18: return {i-1, j-1, k-1};
    case 19: return {i+1, j-1, k-1};
    case 20: return {i+1, j+1, k-1};
    case 21: return {i-1, j+1, k-1};
    case 22: return {i-1, j-1, k-1};
    case 23: return {i+1, j-1, k-1};
    case 24: return {i+1, j+1, k-1};
    case 25: return {i-1, j+1, k-1};
    default: return {-1, -1, -1};
    };
}

/*! Returns the index of a voxel v in a sorted voxel set.  If v is not found
  ! then return -1.  
  !*/
inline size_t VoxelSet::index(const Voxel &v) const {
    auto iter = std::lower_bound(voxels.begin(), voxels.end(), v);
    if (iter == voxels.end() || *iter != v) return -1;
    return std::distance(voxels.begin(), iter);
}
