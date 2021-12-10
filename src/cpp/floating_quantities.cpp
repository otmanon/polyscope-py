#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include "Eigen/Dense"

#include "polyscope/floating_quantity_structure.h"
#include "polyscope/polyscope.h"

#include "utils.h"

namespace py = pybind11;
namespace ps = polyscope;

// For overloaded functions, with C++11 compiler only
template <typename... Args>
using overload_cast_ = pybind11::detail::overload_cast_impl<Args...>;


void bind_floating_quantities(py::module& m) {

  // == General routines
  m.def("remove_floating_quantity", &ps::removeFloatingQuantity, "Remove any floating quantity");
  m.def("remove_all_floating_quantities", &ps::removeAllFloatingQuantities, "Remove all floating quantities");

  // == Floating scalar images

  bindScalarQuantity<ps::FloatingScalarImageQuantity>(m, "FloatingScalarImageQuantity")
      .def("set_show_fullscreen", &ps::FloatingScalarImageQuantity::setShowFullscreen);

  m.def("add_floating_scalar_image", &ps::addFloatingScalarImage<Eigen::VectorXd>, "Add scalar image (expects flat)",
        py::return_value_policy::reference);
  m.def("remove_floating_scalar_image", &ps::removeFloatingScalarImage, "Remove scalar image");
 


  // == Floating color images

  bindColorQuantity<ps::FloatingColorImageQuantity>(m, "FloatingColorImageQuantity")
      .def("set_show_fullscreen", &ps::FloatingColorImageQuantity::setShowFullscreen);

  m.def("add_floating_color_image", &ps::addFloatingColorImage<Eigen::MatrixXd>, "Add color image (expects flatx3)",
        py::return_value_policy::reference);
  m.def("remove_floating_color_image", &ps::removeFloatingColorImage, "Remove color image");

  // bindStructure<ps::FloatingQuantityStructure>(m, "FloatingQuantityStructure");
}
