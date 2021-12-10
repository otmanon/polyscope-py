#include <pybind11/eigen.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "Eigen/Dense"

#include "polyscope/polyscope.h"
#include "polyscope/volume_grid.h"

#include "utils.h"

namespace py = pybind11;
namespace ps = polyscope;

// For overloaded functions, with C++11 compiler only
template <typename... Args>
using overload_cast_ = pybind11::detail::overload_cast_impl<Args...>;


// clang-format off
void bind_volume_grid(py::module& m) {

  // == Helper quantity classes

  // Scalar quantities
  bindScalarQuantity<ps::VolumeGridScalarQuantity>(m, "VolumeGridScalarQuantity")
    .def("set_point_viz_enabled", &ps::VolumeGridScalarQuantity::setPointVizEnabled, "enable point viz")
    .def("get_point_viz_enabled", &ps::VolumeGridScalarQuantity::getPointVizEnabled, "get enable point viz")
    .def("set_isosurface_viz_enabled", &ps::VolumeGridScalarQuantity::setIsosurfaceVizEnabled, "enable isosurface viz")
    .def("get_isosurface_viz_enabled", &ps::VolumeGridScalarQuantity::getIsosurfaceVizEnabled, "get enable isosurface viz")
    .def("set_isosurface_level", &ps::VolumeGridScalarQuantity::setIsosurfaceLevel, "set isosurface level")
    .def("get_isosurface_level", &ps::VolumeGridScalarQuantity::getIsosurfaceLevel, "get isosurface level")
    .def("set_isosurface_color", &ps::VolumeGridScalarQuantity::setIsosurfaceColor, "set isosurface color")
    .def("get_isosurface_color", &ps::VolumeGridScalarQuantity::getIsosurfaceColor, "get isosurface color")
    ;

  // Color quantities
  //bindColorQuantity<ps::VolumeGridColorQuantity>(m, "VolumeGridColorQuantity");

  // Vector quantities
  //bindVectorQuantity<ps::VolumeGridVectorQuantity>(m, "VolumeGridVectorQuantity");


  // == Main class
  bindStructure<ps::VolumeGrid>(m, "VolumeGrid")

    // basics
    .def("n_values", &ps::VolumeGrid::nValues, "# values")
    .def("min_grid_spacing", &ps::VolumeGrid::minGridSpacing, "min grid spacing") 
    .def("position_of_index", 
        static_cast<glm::vec3(ps::VolumeGrid::*)(size_t)const>(&ps::VolumeGrid::positionOfIndex), 
        "position of index")
    .def("position_of_index", 
        static_cast<glm::vec3(ps::VolumeGrid::*)(std::array<size_t,3>)const>(&ps::VolumeGrid::positionOfIndex), 
        "position of index")

    // options
    .def("set_material", &ps::VolumeGrid::setMaterial, "Set material")
    .def("get_material", &ps::VolumeGrid::getMaterial, "Get material")

    // slice planes
    .def("set_ignore_slice_plane", &ps::VolumeGrid::setIgnoreSlicePlane, "Set ignore slice plane")
    .def("get_ignore_slice_plane", &ps::VolumeGrid::getIgnoreSlicePlane, "Get ignore slice plane")
    //.def("set_cull_whole_elements", &ps::VolumeGrid::setCullWholeElements, "Set cull whole elements")
    //.def("get_cull_whole_elements", &ps::VolumeGrid::getCullWholeElements, "Get cull whole elements")

    // quantities
    .def("add_scalar_quantity", 
        static_cast<ps::VolumeGridScalarQuantity*(ps::VolumeGrid::*)(std::string, const Eigen::VectorXd&, ps::DataType)>(&ps::VolumeGrid::addScalarQuantity), 
        "Add a scalar function on the grid",
        py::arg("name"), py::arg("values"), py::arg("data_type")=ps::DataType::STANDARD, py::return_value_policy::reference)
    
    .def("add_scalar_quantity_from_callable", 
        [](ps::VolumeGrid& g, std::string name, const std::function<double(double,double,double)>& func, ps::DataType datatype) 
        { return g.addScalarQuantityFromCallable(name, func, datatype);},
        "Add a scalar function on the grid via callback",
        py::arg("name"), py::arg("values"), py::arg("data_type")=ps::DataType::STANDARD, py::return_value_policy::reference)

    .def("add_scalar_quantity_from_batch_callable", 
        [](ps::VolumeGrid& g, std::string name, const std::function<Eigen::VectorXd(Eigen::MatrixXd)>& func, ps::DataType datatype) 
        // define an internal helper which converts stl <-> Eigen types 
        // (pybind11 does Eigen <-> numpy automatically, so this gets us std <-> numpy at the cost of an extra copy)
        { 
          auto helperFuncConvertBatchTypes = [&](const std::vector<std::array<double, 3>>& arr) {
            Eigen::MatrixXd arrEigen(arr.size(), 3);
            for(size_t i = 0; i < arr.size(); i++) {
              for(size_t j = 0; j < 3; j++) {
                arrEigen(i,j) = arr[i][j];
              }
            }
            return func(arrEigen);
          };
          return g.addScalarQuantityFromBatchCallable(name, helperFuncConvertBatchTypes, datatype);
        },
        "Add a scalar function on the grid via batch callback",
        py::arg("name"), py::arg("values"), py::arg("data_type")=ps::DataType::STANDARD, py::return_value_policy::reference)
    ;
  
  // Static adders and getters
  // (here we use lambda passthru rather than figuring out function overload syntax)
  m.def("register_volume_grid", 
      [](std::string name, std::array<size_t, 3> steps, glm::vec3 bound_min, glm::vec3 bound_max) {
        return ps::registerVolumeGrid(name, steps, bound_min, bound_max);}, 
      py::arg("name"), py::arg("steps"), py::arg("bound_min"), py::arg("bound_max"), "Register a volume grid", py::return_value_policy::reference);
  m.def("register_volume_grid", 
      [](std::string name, size_t steps, glm::vec3 bound_min, glm::vec3 bound_max) {
        return ps::registerVolumeGrid(name, steps, bound_min, bound_max);}, 
      py::arg("name"), py::arg("steps"), py::arg("bound_min"), py::arg("bound_max"), "Register a volume grid", py::return_value_policy::reference);
  m.def("remove_volume_grid", &polyscope::removeVolumeGrid, "Remove a volume grid by name");
  m.def("get_volume_grid", &polyscope::getVolumeGrid, "Get a volume grid by name", py::return_value_policy::reference);
  m.def("has_volume_grid", &polyscope::hasVolumeGrid, "Check for a volume grid by name");
}
