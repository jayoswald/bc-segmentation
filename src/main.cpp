#include <iostream>
#include <algorithm>
#include "timer.h"
#include "graph.h"
#include "voxel_set.h"
#include "vtk_writer.h"
#include "string_tools.h"
#include "watershed.h"
#include "fast_clustering.h"

int main(int argc, char **argv) {
    VoxelSet vs;    

    if (argc != 4) {
        std::cerr << "bc [voxelfile] [n] [outputfile]\n";
        exit(1);
    }
    auto input = argv[1];
    auto n = from_string<int>(argv[2]);
    auto output = std::string(argv[3]);

    vs = read_voxelset(input, n);
    auto timer = Timer();
    auto cb = betweeness_centrality(voxelset_to_graph(vs));
    auto iter = std::max_element(cb.begin(), cb.end());
    std::cout << "Computed centrality in " << timer.elapsed() << " seconds\n"
              << "Maximum centrality value is " << *iter << "\n";
    //vtk_writer writer(vs);
    //writer.write_centrality(cb);
    //writer.write("voxels.vtp");

    std::vector<int> erase;
    double cb_threshold = 0.3;
    for (int i=0; i<cb.size(); ++i) {
        if (cb[i] > cb_threshold) {
            erase.push_back(i);
        }
    }
    std::cout << "Removing " << erase.size() << " bridging particles.\n";
    std::remove_if(vs.voxels.begin(), vs.voxels.end(), [&](const Voxel &v) {
        int index = &v - &vs.voxels[0];
        return std::binary_search(erase.begin(), erase.end(), index);
    });
    write_clusters(output, fast_clustering(vs), vs);
    // Compute distance field (for watershed).
    //auto image = watershed(vs);
    //write_image(image);
}
