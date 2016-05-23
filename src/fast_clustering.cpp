#include "fast_clustering.h"
#include "voxel_set.h"
#include <iostream>
#include <fstream>
#include "string_tools.h"
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>

Clusters fast_clustering(const VoxelSet &vs) {
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
                auto v1 = vs.index(vs.voxels[v0].neighbor(s));
                if (v1 != -1 && !mapped_voxel[v1]) {
                    clusters.back().push_back(v1);
                    mapped_voxel[v1] = true;
                    queue.push_back(v1);
                }
            }
        }
        if (clusters.back().size() < 100) {
            std::cout << "Removed cluster with " 
                      << clusters.back().size() << " voxels.\n";
            clusters.pop_back();
        }
    }
    return clusters;
}

void write_clusters(std::string tag, int i0, const Clusters &c, VoxelSet &vs) {
    for (size_t i=0; i<c.size(); ++i) {
        auto path = tag + "-" + make_string(i0+i, 5) + ".gz";
        std::fstream file(path, std::ios::out|std::ios::binary);
        boost::iostreams::filtering_ostream out;
        out.push(boost::iostreams::gzip_compressor());
        out.push(file);
        for (auto j: c[i]) {
            auto &v = vs.voxels[j];
            out << v.i << " " << v.j << " " << v.k << "\n";
        }
        std::cout << "Wrote " << c[i].size() << " voxels to " << path << "\n";
    }
}

