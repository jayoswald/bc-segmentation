#include "fast_clustering.h"
#include "voxel_set.h"
#include <iostream>
#include <fstream>
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
        if (clusters.back().size() < 100) {
            std::cout << "Removed small cluster with " 
                      << clusters.back().size() << " particles.\n";
            clusters.pop_back();
        }
    }
    std::cout << "Found " << clusters.size() << " clusters.\n";
    return clusters;
}

void write_clusters(std::string path, const Clusters &clusters, VoxelSet &vs) {
    // If file extension is .gz then use gzip to write a compressed
    // file, otherwise just write plain ascii. 
    std::fstream file(path, std::ios::out);
    boost::iostreams::filtering_ostream out;
    if (path.substr(path.length() - 3) == ".gz") {
        out.push(boost::iostreams::gzip_compressor());
    }
    out.push(file);
    int p_ct = 0;
    for (int i=0; i<clusters.size(); ++i) {
        out << "begin cluster" << "; id " << i+1 
            << "; size " << clusters[i].size() << "\n";
        for (auto p: clusters[i]) {
            auto &v = vs.voxels[p];
            out << v.i << " " << v.j << " " << v.k << "\n";
        }
        p_ct += clusters[i].size();
        out << "end cluster\n";
    }
    std::cout << "Wrote " << p_ct << " particles, and "
              << clusters.size() << " clusters.";
}

