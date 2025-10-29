#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/ndarray.h>

#include "clipper2/clipper.h"

namespace nb = nanobind;
using namespace Clipper2Lib;

NB_MODULE(pyclipper2, m) {
    m.doc() = "Python bindings for Clipper2 library";

    // Enums
    nb::enum_<ClipType>(m, "ClipType")
        .value("NoClip", ClipType::NoClip)
        .value("Intersection", ClipType::Intersection)
        .value("Union", ClipType::Union)
        .value("Difference", ClipType::Difference)
        .value("Xor", ClipType::Xor);

    nb::enum_<FillRule>(m, "FillRule")
        .value("EvenOdd", FillRule::EvenOdd)
        .value("NonZero", FillRule::NonZero)
        .value("Positive", FillRule::Positive)
        .value("Negative", FillRule::Negative);

    nb::enum_<JoinType>(m, "JoinType")
        .value("Square", JoinType::Square)
        .value("Bevel", JoinType::Bevel)
        .value("Round", JoinType::Round)
        .value("Miter", JoinType::Miter);

    nb::enum_<EndType>(m, "EndType")
        .value("Polygon", EndType::Polygon)
        .value("Joined", EndType::Joined)
        .value("Butt", EndType::Butt)
        .value("Square", EndType::Square)
        .value("Round", EndType::Round);

    // Core types - Point64, PointD
    nb::class_<Point64>(m, "Point64")
        .def(nb::init<int64_t, int64_t>())
        .def_rw("x", &Point64::x)
        .def_rw("y", &Point64::y)
        .def("__repr__", [](const Point64 &p) {
            return nb::str("Point64(x={}, y={})").format(p.x, p.y);
        });

    nb::class_<PointD>(m, "PointD")
        .def(nb::init<double, double>())
        .def_rw("x", &PointD::x)
        .def_rw("y", &PointD::y)
        .def("__repr__", [](const PointD &p) {
            return nb::str("PointD(x={}, y={})").format(p.x, p.y);
        });

    // Rect types
    nb::class_<Rect64>(m, "Rect64")
        .def(nb::init<int64_t, int64_t, int64_t, int64_t>())
        .def_rw("left", &Rect64::left)
        .def_rw("top", &Rect64::top)
        .def_rw("right", &Rect64::right)
        .def_rw("bottom", &Rect64::bottom);

    nb::class_<RectD>(m, "RectD")
        .def(nb::init<double, double, double, double>())
        .def_rw("left", &RectD::left)
        .def_rw("top", &RectD::top)
        .def_rw("right", &RectD::right)
        .def_rw("bottom", &RectD::bottom);

    // ClipperOffset
    nb::class_<ClipperOffset>(m, "ClipperOffset")
        .def(nb::init<double, double, bool>(),
             nb::arg("miter_limit") = 2.0,
             nb::arg("arc_tolerance") = 0.0,
             nb::arg("reverse_solution") = false)
        .def("add_path", &ClipperOffset::AddPath)
        .def("add_paths", &ClipperOffset::AddPaths)
        .def("execute", 
             nb::overload_cast<double, Paths64&>(&ClipperOffset::Execute))
        .def("clear", &ClipperOffset::Clear);

    // Utility functions
    m.def("area", 
          nb::overload_cast<const Path64&>(&Area<int64_t>),
          "Calculate area of a path");
    
    m.def("area",
          nb::overload_cast<const PathD&>(&Area<double>),
          "Calculate area of a path");
    
    m.def("is_positive",
          nb::overload_cast<const Path64&>(&IsPositive<int64_t>),
          "Check if path is positively oriented");
    
    m.def("is_positive",
          nb::overload_cast<const PathD&>(&IsPositive<double>),
          "Check if path is positively oriented");

    // Module constants
    m.attr("VERSION") = CLIPPER2_VERSION;
}