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
        .value("NO_CLIP", ClipType::NoClip)
        .value("INTERSECTION", ClipType::Intersection)
        .value("UNION", ClipType::Union)
        .value("DIFFERENCE", ClipType::Difference)
        .value("XOR", ClipType::Xor);

    nb::enum_<FillRule>(m, "FillRule")
        .value("EVEN_ODD", FillRule::EvenOdd)
        .value("NON_ZERO", FillRule::NonZero)
        .value("POSITIVE", FillRule::Positive)
        .value("NEGATIVE", FillRule::Negative);

    nb::enum_<JoinType>(m, "JoinType")
        .value("SQUARE", JoinType::Square)
        .value("BEVEL", JoinType::Bevel)
        .value("ROUND", JoinType::Round)
        .value("MITER", JoinType::Miter);

    nb::enum_<EndType>(m, "EndType")
        .value("POLYGON", EndType::Polygon)
        .value("JOINED", EndType::Joined)
        .value("BUTT", EndType::Butt)
        .value("SQUARE", EndType::Square)
        .value("ROUND", EndType::Round);

    nb::enum_<PathType>(m, "PathType")
        .value("SUBJECT", PathType::Subject)
        .value("CLIP", PathType::Clip);

    nb::enum_<JoinWith>(m, "JoinWith")
        .value("NO_JOIN", JoinWith::NoJoin)
        .value("LEFT", JoinWith::Left)
        .value("RIGHT", JoinWith::Right);

    nb::enum_<PointInPolygonResult>(m, "PointInPolygonResult")
        .value("IS_ON", PointInPolygonResult::IsOn)
        .value("IS_INSIDE", PointInPolygonResult::IsInside)
        .value("IS_OUTSIDE", PointInPolygonResult::IsOutside);

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

    // We must write one for Path64 (int64) and one for PathD (double)
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

    m.def("point_in_polygon",
          nb::overload_cast<const Point64&, const Path64&>(&PointInPolygon<int64_t>),
          "Check if point is in polygon",
          nb::arg("pt"), nb::arg("polygon"));

    m.def("point_in_polygon",
          nb::overload_cast<const PointD&, const PathD&>(&PointInPolygon<double>),
          "Check if point is in polygon",
          nb::arg("pt"), nb::arg("polygon"));

    // Module constants
    m.attr("VERSION") = CLIPPER2_VERSION;
}