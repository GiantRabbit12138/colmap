#include "colmap/geometry/homography_matrix.h"

#include "colmap/util/logging.h"

#include "pycolmap/helpers.h"
#include "pycolmap/pybind11_extension.h"

#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

using namespace colmap;
using namespace pybind11::literals;
namespace py = pybind11;

py::dict PyPoseFromHomographyMatrix(
    const Eigen::Matrix3d& H,
    const Eigen::Matrix3d& K1,
    const Eigen::Matrix3d& K2,
    const std::vector<Eigen::Vector3d>& cam_rays1,
    const std::vector<Eigen::Vector3d>& cam_rays2) {
  py::gil_scoped_release release;
  Rigid3d cam2_from_cam1;
  Eigen::Vector3d normal;
  std::vector<Eigen::Vector3d> points3D;
  PoseFromHomographyMatrix(
      H, K1, K2, cam_rays1, cam_rays2, &cam2_from_cam1, &normal, &points3D);
  py::gil_scoped_acquire acquire;
  return py::dict("cam2_from_cam1"_a = cam2_from_cam1,
                  "normal"_a = normal,
                  "points3D"_a = points3D);
}

void BindHomographyMatrixGeometry(py::module& m) {
  m.def("pose_from_homography_matrix",
        &PyPoseFromHomographyMatrix,
        "H"_a,
        "K1"_a,
        "K2"_a,
        "cam_rays1"_a,
        "cam_rays2"_a,
        "Recover the most probable pose from the given homography matrix using "
        "the cheirality check.");
  DefDeprecation(m, "homography_decomposition", "pose_from_homography_matrix");
}
