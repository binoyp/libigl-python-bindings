#include "default_types.h"
#include <igl/sharp_edges.h>
#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
#include <nanobind/eigen/dense.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/vector.h>

namespace nb = nanobind;
using namespace nb::literals;

namespace pyigl
{
  // Simple overload: returns just SE (the sharp edge vertex pairs)
  auto sharp_edges(
    const nb::DRef<const Eigen::MatrixXN> &V,
    const nb::DRef<const Eigen::MatrixXI> &F,
    const Numeric angle)
  {
    Eigen::MatrixXI SE;
    igl::sharp_edges(V, F, angle, SE);
    return SE;
  }

  // Full overload: returns SE plus all the edge-map data
  auto sharp_edges_full(
    const nb::DRef<const Eigen::MatrixXN> &V,
    const nb::DRef<const Eigen::MatrixXI> &F,
    const Numeric angle)
  {
    Eigen::MatrixXI SE;
    Eigen::MatrixXI E;
    Eigen::MatrixXI uE;
    Eigen::VectorXI EMAP;
    std::vector<std::vector<Integer>> uE2E;
    std::vector<Integer> sharp;
    igl::sharp_edges(V, F, angle, SE, E, uE, EMAP, uE2E, sharp);
    return std::make_tuple(SE, E, uE, EMAP, uE2E, sharp);
  }
}

// Bind the wrapper to the Python module
void bind_sharp_edges(nb::module_ &m)
{
  m.def(
    "sharp_edges",
    &pyigl::sharp_edges,
    "V"_a,
    "F"_a,
    "angle"_a,
R"(Given a mesh, compute sharp edges.

@param[in] V  #V by 3 list of vertex positions
@param[in] F  #F by 3 list of triangle mesh indices into V
@param[in] angle  dihedral angle considered sharp (e.g., igl.PI * 0.11)
@param[out] SE  #SE by 2 list of sharp edge indices into V)");
  m.def(
    "sharp_edges_full",
    &pyigl::sharp_edges_full,
    "V"_a,
    "F"_a,
    "angle"_a,
R"(Given a mesh, compute sharp edges with full edge-map output.

@param[in] V  #V by 3 list of vertex positions
@param[in] F  #F by 3 list of triangle mesh indices into V
@param[in] angle  dihedral angle considered sharp (e.g., igl.PI * 0.11)
@param[out] SE  #SE by 2 list of sharp edge indices into V
@param[out] E  #F*3 by 2 list of all directed edges
@param[out] uE  #uE by 2 list of unique undirected edges
@param[out] EMAP  #F*3 list of indices into uE
@param[out] uE2E  #uE list of lists of indices into E of coexisting edges
@param[out] sharp  #SE list of indices into uE of sharp edges)");
}
