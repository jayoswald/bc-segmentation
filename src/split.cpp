#include <iostream>
#include <fstream>
#include "voxel_set.h"

typedef std::vector<std::vector<size_t>> Clusters;

Clusters clustering(const VoxelSet &vs);

int main(int narg, char **argv) {
    if (narg != 3) {
        std::cerr << "split [voxelfile] [output].\n";
        exit(1);
    }
    auto vs = read_voxelset(argv[1]);
    auto clusters = clustering(vs);

    std::fstream out(argv[2], std::ios::out);
    int c_ct = 0, p_ct = 0;
    for (auto &cluster: clusters) {
        if (cluster.size() > 100) {
            c_ct += 1;
            out << "begin cluster; size " << cluster.size() << "\n";
            for (auto i: cluster) {
                auto &v = vs.voxels[i];
                out << v.i << " " << v.j << " " << v.k << "\n";
                p_ct += 1;
            }
            out << "end cluster\n";
        }
    }
    std::cout << "Wrote " << p_ct << " particles\n";
    std::cout << "Wrote " << c_ct << " out of " 
              << clusters.size() << " clusters.\n";
}

Clusters clustering(const VoxelSet &vs) {
    Clusters clusters;
    std::vector<bool> mapped_voxel(vs.voxels.size(), false);

    for (size_t i=0; i<vs.voxels.size(); ++i) {
        if (mapped_voxel[i]) continue;
        // Voxel i begins a new voxel.
        clusters.push_back({i});
        mapped_voxel[i] = true;

        // Do a breadth-first search for all continuous voxels.
        std::vector<size_t> queue = {i};
        while (!queue.empty()) {
            auto v0 = queue.back();
            queue.pop_back();
            for (int s=0; s<6; ++s) {
                auto neigh = vs.voxels[v0];
                if      (s==0) neigh.i += 1;
                else if (s==1) neigh.i -= 1;
                else if (s==2) neigh.j += 1;
                else if (s==3) neigh.j -= 1;
                else if (s==4) neigh.k += 1;
                else if (s==5) neigh.k -= 1;
                auto v1 = vs.index(neigh);
                if (v1 != -1 && !mapped_voxel[v1]) {
                    clusters.back().push_back(v1);
                    mapped_voxel[v1] = true;
                    queue.push_back(v1);
                }
            }
        }
    }
    std::cout << "Found " << clusters.size() << " clusters.\n";
    return clusters;
}

