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

//! Reads a set of voxels from an input file containing multiple clusters.
std::vector<Voxel> read_voxelset(std::string path, int i) {
    std::vector<Voxel> voxels;
    std::fstream fid(path, std::ios::in);
    int ct = 0;
    while (fid) {
        auto line = read_line(fid);
        if (startswith(line, "begin cluster")) {
            ct += 1;
        }
        else if (startswith(line, "end cluster") && ct == i) {
            break;
        }
        else if (ct == i) {
            Voxel v;
            std::istringstream ss(line);
            ss >> v.i >> v.j >> v.k;
            if (fid) {
                voxels.push_back(v);
            }
        }
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
            auto nv = index(voxels, voxels[v].neighbor(n));
            if (nv != -1) {
                g.neighbors[v].push_back(nv);
                num_neighbors += 1;
            }
        }
    }
    std::cout << "Read " << voxels.size() << " voxels connected with "
              << num_neighbors << " neighbors.\n";
    return g;
}

