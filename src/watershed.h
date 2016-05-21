#pragma once
#include <vector>
#include <iostream>
class VoxelSet;

// 3D grid.
class Image {
public:
    double operator()(int i, int j, int k) const {
        return data[i + j*L + k*L*M];
    };
    double& operator()(int i, int j, int k) {
        return data[i + j*L + k*L*M];
    };
    std::size_t L, M, N;
    std::vector<double> data;
private:
};

// Converts a (sparse) list of voxels to 3D binary image.
Image voxels2image(const VoxelSet &vs);

Image watershed(const VoxelSet &vs);

