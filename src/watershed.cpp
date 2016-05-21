#include "watershed.h"
#include "voxel_set.h"
#include <cmath>
#include <iostream>

// Converts a (sparse) list of voxels to 3D binary image.
Image voxels2image(const VoxelSet &vs) {
    Image image;
    image.L = vs.xhi-vs.xlo+1;
    image.M = vs.yhi-vs.ylo+1;
    image.N = vs.zhi-vs.zlo+1;

    std::cout << "Image is " << image.L << "x" << image.M << "x" << image.N << "\n";
    image.data.assign(image.L*image.M*image.N, 0.0);
    for (auto v: vs.voxels) {
        image(v.i-vs.xlo, v.j-vs.ylo, v.k-vs.zlo) = 1.0;
    }
    return image;
}

Image watershed(const VoxelSet &vs) {    
    auto image = voxels2image(vs);
    // Step 1: forward scan 
    for (int k=0; k<image.N; ++k) {
        for (int j=0; j<image.M; ++j) {
            double df = image.L;
            for (int i=0; i<image.L; ++i) {
                if (image(i,j,k) > 0.0) df += 1;
                else df = 0;
                image(i,j,k) = df*df;
            }
        }
    }
    // Backward scan.
    for (int k=0; k<image.N; ++k) {
        for (int j=0; j<image.M; ++j) {
            double db = image.L;
            for (int i=image.L-1; i>=0; --i) {
                if (image(i,j,k) > 0) db += 1;
                else db = 0;
                image(i,j,k) = std::min(image(i,j,k), db*db);
            }
        }
    }
    // Step 2
    std::vector<double> buff(std::max(image.M, image.N));
    for (int k=0; k<image.N; ++k) {
        for (int i=0; i<image.L; ++i) {
            for (int j=0; j<image.M; j++) {
                buff[j] = image(i,j,k);
            }
            for (int j=0; j<image.M; ++j) {
                auto d = buff[j];
                if (d > 0) {
                    int rmax = int(sqrt(d)) + 1;
                    int rstart = std::min(rmax, j);
                    int rend = std::min(rmax, int(image.M-j-1));
                    for (int n = -rstart; n<=rend; ++n) {
                        auto w = buff[j+n] + n*n;
                        if (w < d) d = w;
                    }
                }
                image(i,j,k) = d;
            }
        }
    }

    // Step 3
    for (int i=0; i<image.L; ++i) {
        for (int j=0; j<image.M; j++) {
            // Make a copy of the row k.
            for (int k=0; k<image.N; ++k) {
                buff[k] = image(i,j,k);
            }
            for (int k=0; k<image.N; ++k) {
                auto d = buff[k];
                if (d > 0) {
                    int rmax = int(sqrt(d)) + 1;
                    int rstart = std::min(rmax, k);
                    int rend = std::min(rmax, int(image.N-k-1));
                    for (int n = -rstart; n<=rend; ++n) {
                        auto w = buff[k+n] + n*n;
                        if (w < d) d = w;
                    }
                }
                image(i,j,k) = d;
            }
        }
    }
    for (auto &x: image.data) x = sqrt(x);
    return image;
}
    
