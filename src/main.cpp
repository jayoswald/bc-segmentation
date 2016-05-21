#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "timer.h"
#include "graph.h"
#include "voxel_set.h"
#include "vtk_writer.h"
#include "string_tools.h"

int main(int argc, char **argv) {
    VoxelSet voxels;    
    if (argc == 2) {
        voxels = read_voxelset(argv[1]); 
    }
    else if (argc == 3) {
        voxels = read_voxelset(argv[1], from_string<int>(argv[2])); 
    }
    else {
        std::cerr << "bc [voxelfile]\n";
        std::cerr << "bc [voxelfile] [n]\n";
        exit(1);
    }
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

