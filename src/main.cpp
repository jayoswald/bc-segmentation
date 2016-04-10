#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "timer.h"
#include "graph.h"
#include "voxel_set.h"
#include "vtk_writer.h"

int main(int argc, char **argv) {

    if (argc != 2) {
        std::cerr << "bc [voxelfile]\n";
        exit(1);
    }

    auto voxels = read_voxelset(argv[1]); 
    auto graph = voxelset_to_graph(voxels);
    auto timer = Timer();
    auto cb = betweeness_centrality(graph);
    std::cout << "Computed centrality in " << timer.elapsed() << " seconds\n";

    std::cout << "Maximum centrality value is "
              << *std::max_element(cb.begin(), cb.end()) << "\n";

    vtk_writer writer(voxels);
    writer.write_centrality(cb);
    writer.write("voxels.vtp");
}

