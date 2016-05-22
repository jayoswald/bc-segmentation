#include <iostream>
#include "voxel_set.h"
#include "fast_clustering.h"

Clusters clustering(const VoxelSet &vs);

int main(int narg, char **argv) {
    if (narg != 3) {
        std::cerr << "split [voxelfile] [output].\n";
        exit(1);
    }
    auto vs = read_voxelset(argv[1]);
    auto clusters = fast_clustering(vs);
    std::string path = argv[2]; 
    write_clusters(path, clusters, vs);
}
