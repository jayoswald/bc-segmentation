#pragma once
#include <vector>
#include <string>
#include <memory>

class Voxel;

class vtkData;

class vtk_writer {
public:
    vtk_writer(const std::vector<Voxel> &voxels);
    ~vtk_writer();

    void write_centrality(const std::vector<double> &c);
    void write(std::string path);


private:
    struct impl;
    std::unique_ptr<impl> _pimpl;
};

