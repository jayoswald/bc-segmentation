#pragma once
#include <vector>
#include <string>

class VoxelSet;
typedef std::vector<std::vector<size_t>> Clusters;

Clusters fast_clustering(const VoxelSet &vs);
void write_clusters(std::string path, const Clusters &clusters, VoxelSet &vs);

