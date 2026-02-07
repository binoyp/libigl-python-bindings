#include "default_types.h"
#include <igl/exterior_edges.h>
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/eigen/dense.h>
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/eigen/dense.h>

namespace nb = nanobind;
using namespace nb::literals;

namespace pyigl
{
  auto exterior_edges(
    const nb::DRef<const Eigen::MatrixXI> &F)
  {
    Eigen::MatrixXI E;
    igl::exterior_edges(F, E);
    return E;
  }
}

// Bind the wrapper to the Python module
void bind_exterior_edges(nb::module_ &m)
{
  m.def(
    "exterior_edges",
    &pyigl::exterior_edges,
    "F"_a,
R"(Determines boundary "edges" and also edges with an odd number of
occurrences where seeing edge (i,j) counts as +1 and seeing the opposite
edge (j,i) counts as -1.

@param[in] F  #F by simplex_size list of "faces"
@return E  #E by simplex_size-1 list of exterior edges)");
}
