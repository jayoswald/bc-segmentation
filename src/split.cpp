#include <iostream>
#include "voxel_set.h"
#include "fast_clustering.h"

Clusters clustering(const VoxelSet &vs);

int main(int narg, char **argv) {
    if (narg != 2) {
        std::cerr << "split [voxelfile].\n";
        exit(1);
    }
    auto vs = read_voxelset(argv[1]);
    write_clusters("p", 1, fast_clustering(vs), vs);
}
