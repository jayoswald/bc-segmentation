#pragma once
#include <vector>
#include <string>
#include <memory>

class VoxelSet;
class vtkData;
class Image;

class vtk_writer {
public:
    vtk_writer(const VoxelSet &vs);
    ~vtk_writer();

    void write_centrality(const std::vector<double> &c);
    void write(std::string path);

private:
    struct impl;
    std::unique_ptr<impl> _pimpl;
};

// Writes vtk rectilinear grid for the distance function.
void write_image(const Image& image);
