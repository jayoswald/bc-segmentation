#include "voxel_set.h"
#include <algorithm>
#include <fstream>
#include "string_tools.h"
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>

int VoxelSet::coordination(const Voxel &v) const {
    int c = 0;
    for (int i=0; i<6; ++i) {
        if (index(v.neighbor(i)) > -1) {
            c += 1;
        }
    }
    return c;
}

// Reads a set of voxels from an input file containing multiple clusters.
VoxelSet read_voxelset(std::string path, int i) {
    VoxelSet vs;
    std::ifstream file(path, std::ios::in|std::ios::binary);
    boost::iostreams::filtering_istream in;
    if (path.substr(path.length() - 3) == ".gz") {
        in.push(boost::iostreams::gzip_decompressor());
    }
    in.push(file);
    int ct = 0;
    while (in) {
        auto line = read_line(in);
        if (startswith(line, "begin cluster")) {
            ct += 1;
        }
        else if (startswith(line, "end cluster")) {
            if (ct == i) break;
        }
        else if (ct == i || i == 0) {
            Voxel v;
            std::istringstream ss(line);
            ss >> v.i >> v.j >> v.k;
            if (in) {
                vs.voxels.push_back(v);
                if (vs.voxels.size() == 1) {
                    vs.xlo = vs.xhi = v.i;
                    vs.ylo = vs.yhi = v.j;
                    vs.zlo = vs.zhi = v.k;
                }
                else {
                    vs.xlo = std::min(vs.xlo, v.i);
                    vs.xhi = std::max(vs.xhi, v.i);
                    vs.ylo = std::min(vs.ylo, v.j);
                    vs.yhi = std::max(vs.yhi, v.j);
                    vs.zlo = std::min(vs.zlo, v.k);
                    vs.zhi = std::max(vs.zhi, v.k);
                }
            }
        }
    }
    std::sort(vs.voxels.begin(), vs.voxels.end());
    std::cout << "Read " << vs.voxels.size() << " voxels.\n";
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
    return g;
}

