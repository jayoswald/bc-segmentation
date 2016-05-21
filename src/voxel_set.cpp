#include "voxel_set.h"
#include <algorithm>
#include <fstream>
#include "string_tools.h"

//! Reads a set of voxels from an input file containing multiple clusters.
VoxelSet read_voxelset(std::string path, int i) {
    VoxelSet vs;
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
                vs.voxels.push_back(v);
                if (vs.voxels.size() == 1) {
                    vs.xlo = vs.xhi = v.i;
                    vs.ylo = vs.yhi = v.j;
                    vs.zlo = vs.zhi = v.k;
                }
                else {
                    vs.xlo = std::min(vs.xlo, v.i);
                    vs.xhi = std::max(vs.xlo, v.i);
                    vs.ylo = std::min(vs.ylo, v.j);
                    vs.yhi = std::max(vs.ylo, v.j);
                    vs.zlo = std::min(vs.zlo, v.k);
                    vs.zhi = std::max(vs.zlo, v.k);
                }
            }
        }
    }
    std::sort(vs.voxels.begin(), vs.voxels.end());
    return vs;
}

Graph voxelset_to_graph(const VoxelSet &vs) {
    Graph g;
    g.neighbors.assign(vs.voxels.size(), {});
    int num_neighbors = 0;
    for (int v=0; v<vs.voxels.size(); ++v) {
        g.vertices.push_back(v);
        for (int n=0; n<6; n++) {
            auto nv = vs.index(vs.voxels[v].neighbor(n));
            if (nv != -1) {
                g.neighbors[v].push_back(nv);
                num_neighbors += 1;
            }
        }
    }
    std::cout << "Read " << vs.voxels.size() << " voxels connected with "
              << num_neighbors << " neighbors.\n";
    return g;
}

