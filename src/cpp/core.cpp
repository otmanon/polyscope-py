#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include "Eigen/Dense"

#include "polyscope/affine_remapper.h"
#include "polyscope/polyscope.h"
#include "polyscope/surface_parameterization_enums.h"
#include "polyscope/view.h"

namespace py = pybind11;
namespace ps = polyscope;

// For overloaded functions, with C++11 compiler only
template <typename... Args>
using overload_cast_ = pybind11::detail::overload_cast_impl<Args...>;


// Forward-declare bindings from other files
void bind_surface_mesh(py::module& m);
void bind_point_cloud(py::module& m);
void bind_curve_network(py::module& m);

// Actual binding code
// clang-format off
PYBIND11_MODULE(polyscope_bindings, m) {
  m.doc() = "Polyscope low-level bindings";
  
  
  // === Basic flow 
  m.def("init", &ps::init, "Initialize Polyscope");
  m.def("show", &ps::show, "Show the Polyscope GUI (blocks until UI is exited)", 
      py::arg("forFrames") = std::numeric_limits<size_t>::max());

  // === Structure management
  m.def("remove_all_structures", &ps::removeAllStructures, "Remove all structures from polyscope");
  
  // === Screenshots
  m.def("screenshot", overload_cast_<bool>()(&ps::screenshot), "Take a screenshot");
  m.def("named_screenshot", overload_cast_<std::string, bool>()(&ps::screenshot), "Take a screenshot");

  // === Small options
  m.def("set_program_name", [](std::string x) { ps::options::programName = x; });
  m.def("set_verbosity", [](int x) { ps::options::verbosity = x; });
  m.def("set_print_prefix", [](std::string x) { ps::options::printPrefix = x; });
  m.def("set_max_fps", [](int x) { ps::options::maxFPS = x; });
  m.def("set_errors_throw_exceptions", [](bool x) { ps::options::errorsThrowExceptions = x; });
  m.def("set_use_prefs_file", [](bool x) { ps::options::usePrefsFile = x; });
  m.def("set_always_redraw", [](bool x) { ps::options::alwaysRedraw = x; });
  m.def("set_autocenter_structures", [](bool x) { ps::options::autocenterStructures = x; });
  m.def("set_autoscale_structures", [](bool x) { ps::options::autoscaleStructures = x; });
  m.def("set_navigation_style", [](ps::view::NavigateStyle x) { ps::view::style = x; });
  m.def("set_up_dir", [](ps::view::UpDir x) { ps::view::upDir = x; });
  
  // === Messages
  m.def("info", ps::info, "Send an info message");
  m.def("warning", ps::warning, "Send a warning message");
  m.def("error", ps::error, "Send an error message");
  m.def("terminating_error", ps::terminatingError, "Send a terminating error message");

  // === Enums
  
  py::enum_<ps::view::NavigateStyle>(m, "NavigateStyle")
    .value("turntable", ps::view::NavigateStyle::Turntable)
    .value("free", ps::view::NavigateStyle::Free)
    .value("planar", ps::view::NavigateStyle::Planar)
    .value("arcball", ps::view::NavigateStyle::Arcball)
    .export_values(); 
  
  py::enum_<ps::view::UpDir>(m, "UpDir")
    .value("x_up", ps::view::UpDir::XUp)
    .value("y_up", ps::view::UpDir::YUp)
    .value("z_up", ps::view::UpDir::ZUp)
    .export_values(); 

  py::enum_<ps::DataType>(m, "DataType")
    .value("standard", ps::DataType::STANDARD)
    .value("symmetric", ps::DataType::SYMMETRIC)
    .value("magnitude", ps::DataType::MAGNITUDE)
    .export_values(); 

  py::enum_<ps::VectorType>(m, "VectorType")
    .value("standard", ps::VectorType::STANDARD)
    .value("ambient", ps::VectorType::AMBIENT)
    .export_values(); 
  
  py::enum_<ps::ParamCoordsType>(m, "ParamCoordsType")
    .value("unit", ps::ParamCoordsType::UNIT)
    .value("world", ps::ParamCoordsType::WORLD)
    .export_values(); 
  
  py::enum_<ps::ParamVizStyle>(m, "ParamVizStyle")
    .value("checker", ps::ParamVizStyle::CHECKER)
    .value("grid", ps::ParamVizStyle::GRID)
    .value("local_check", ps::ParamVizStyle::LOCAL_CHECK)
    .value("local_rad", ps::ParamVizStyle::LOCAL_RAD)
    .export_values(); 


  // === Mini bindings for a little bit of glm
  py::class_<glm::vec3>(m, "glm_vec3").
    def(py::init<float, float, float>())
   .def("as_tuple",
        [](const glm::vec3& x) {
        return std::tuple<float, float, float>(x[0], x[1], x[2]);
        });

  // === Bind structures defined in other files
  bind_surface_mesh(m);
  bind_point_cloud(m);
  bind_curve_network(m);

}

// clang-format on