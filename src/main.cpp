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

    if (argc != 3) {
        std::cerr << "bc [voxelfile] [i0]\n";
        exit(1);
    }
    auto input = argv[1];
    auto i0 = from_string<int>(argv[2]);

    vs = read_voxelset(input);
    auto timer = Timer();
    auto cb = betweeness_centrality(voxelset_to_graph(vs));
    auto max_cb = *std::max_element(cb.begin(), cb.end());
    std::cout << "Computed centrality in " << timer.elapsed() << " seconds\n"
              << "Maximum centrality value is " << max_cb << "\n";
    std::vector<int> erase;
    double cb_threshold = 0.2;
    for (int i=0; i<cb.size(); ++i) {
        if (cb[i] > cb_threshold &&
            (vs.coordination(vs.voxels[i]) < 6 || cb[i] == max_cb)) {
            erase.push_back(i);
        }
    }
    auto pred = [&](const Voxel &v) {
        int index = &v - &vs.voxels[0];
        return std::binary_search(erase.begin(), erase.end(), index);
    };
    vs.voxels.erase(std::remove_if(vs.voxels.begin(), vs.voxels.end(), pred), 
                    vs.voxels.end());
    std::cout << "Removed " << erase.size() << " bridging voxels.\n";
    write_clusters("p", i0, fast_clustering(vs), vs);
}
