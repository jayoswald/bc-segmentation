#include "vtk_writer.h"
#include "voxel_set.h"
#undef __DEPRECATED
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkCellData.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkDoubleArray.h>
#include <vtkVertex.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolyVertex.h>
#define __DEPRECATED

struct vtk_writer::impl {
    vtkSmartPointer<vtkPolyData> polydata;
};

vtk_writer::vtk_writer(const VoxelSet &vs) 
: _pimpl(new vtk_writer::impl) {
    auto points = vtkSmartPointer<vtkPoints>::New();
    auto vertices = vtkSmartPointer<vtkPolyVertex>::New();
    for (auto &voxel: vs.voxels) {
        double h = 1.0;
        double coord[] = {h*voxel.i, h*voxel.j, h*voxel.k};
        points->InsertNextPoint(coord);
        vertices->GetPointIds()->InsertNextId(points->GetNumberOfPoints()-1);
    }
    auto cells = vtkSmartPointer<vtkCellArray>::New();
    cells->InsertNextCell(vertices);
    _pimpl->polydata = vtkSmartPointer<vtkPolyData>::New();
    _pimpl->polydata->SetPoints(points);
    _pimpl->polydata->SetVerts(cells);
}

vtk_writer::~vtk_writer() {}

void vtk_writer::write_centrality(const std::vector<double> &c) {
    auto array = vtkSmartPointer<vtkDoubleArray>::New();
    array->SetNumberOfComponents(1);
    array->SetName("c_b");
    array->SetNumberOfValues(c.size());
    for (int i=0; i<c.size(); ++i) {
        array->SetValue(i, c[i]);
    }
    _pimpl->polydata->GetPointData()->AddArray(array);
}

void vtk_writer::write(std::string path) {
    auto writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
    writer->SetFileName(path.c_str());
    writer->SetInput(_pimpl->polydata);
    writer->Write();
}
