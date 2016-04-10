#include "voxel_set.h"
#include <algorithm>
#include <fstream>
#include "string_tools.h"

std::vector<Voxel> read_voxelset(std::string path) {
    std::vector<Voxel> voxels; 
    std::fstream fid(path, std::ios::in);
    while (fid) {
        Voxel v;
        fid >> v.i >> v.j >> v.k;
        if (fid) voxels.push_back(v);
    }
    std::sort(voxels.begin(), voxels.end());
    return voxels;
}

Graph voxelset_to_graph(std::vector<Voxel> &voxels) {

    Graph g;
    g.neighbors.assign(voxels.size(), {});

    int num_neighbors = 0;
    for (int v=0; v<voxels.size(); ++v) {
        g.vertices.push_back(v);
        for (int n=0; n<6; n++) {
            auto nv = voxels[v].neighbor(n);
            auto iter = std::lower_bound(voxels.begin(), voxels.end(), nv);
            if (iter->i == nv.i && iter->j == nv.j && iter->k == nv.k) {
                g.neighbors[v].push_back(std::distance(voxels.begin(), iter));
                num_neighbors += 1;
            }
        }
    }
    std::cout << "Read " << voxels.size() << " voxels connected with "
              << num_neighbors << " neighbors.\n";

    return g;
}

