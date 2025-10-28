#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/ndarray.h>

// Include the Clipper2 headers
#include "external/clipper2/CPP/Clipper2Lib/include/clipper2/clipper.export.h"
#include "external/clipper2/CPP/Clipper2Lib/include/clipper2/clipper.h"

namespace nb = nanobind;
using namespace Clipper2Lib;

NB_MODULE(_clipper2, m) {
    m.doc() = "Python bindings for Clipper2 library";

    // Enums
    nb::enum_<ClipType>(m, "ClipType")
        .value("NoClip", ClipType::None)
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
        .def("__repr__", [](const Point64& p) {
            return "Point64(x=" + std::to_string(p.x) + 
                   ", y=" + std::to_string(p.y) + ")";
        });

    nb::class_<PointD>(m, "PointD")
        .def(nb::init<double, double>())
        .def_rw("x", &PointD::x)
        .def_rw("y", &PointD::y)
        .def("__repr__", [](const PointD& p) {
            return "PointD(x=" + std::to_string(p.x) + 
                   ", y=" + std::to_string(p.y) + ")";
        });

    // Path types (std::vector wrappers)
    nb::class_<Path64>(m, "Path64")
        .def(nb::init<>())
        .def("__len__", [](const Path64& p) { return p.size(); })
        .def("__getitem__", [](const Path64& p, size_t i) { return p[i]; })
        .def("append", [](Path64& p, const Point64& pt) { p.push_back(pt); });

    nb::class_<PathD>(m, "PathD")
        .def(nb::init<>())
        .def("__len__", [](const PathD& p) { return p.size(); })
        .def("__getitem__", [](const PathD& p, size_t i) { return p[i]; })
        .def("append", [](PathD& p, const PointD& pt) { p.push_back(pt); });

    nb::class_<Paths64>(m, "Paths64")
        .def(nb::init<>())
        .def("__len__", [](const Paths64& p) { return p.size(); })
        .def("__getitem__", [](const Paths64& p, size_t i) { return p[i]; })
        .def("append", [](Paths64& p, const Path64& path) { p.push_back(path); });

    nb::class_<PathsD>(m, "PathsD")
        .def(nb::init<>())
        .def("__len__", [](const PathsD& p) { return p.size(); })
        .def("__getitem__", [](const PathsD& p, size_t i) { return p[i]; })
        .def("append", [](PathsD& p, const PathD& path) { p.push_back(path); });

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

    // Main Clipper64 class
    nb::class_<Clipper64>(m, "Clipper64")
        .def(nb::init<>())
        .def("add_subject", &Clipper64::AddSubject)
        .def("add_open_subject", &Clipper64::AddOpenSubject)
        .def("add_clip", &Clipper64::AddClip)
        .def("execute", 
             nb::overload_cast<ClipType, FillRule, Paths64&, Paths64&>(&Clipper64::Execute))
        .def("clear", &Clipper64::Clear)
        .def("preserve_collinear", &Clipper64::PreserveCollinear)
        .def("reverse_solution", &Clipper64::ReverseSolution);

    // Main ClipperD class  
    nb::class_<ClipperD>(m, "ClipperD")
        .def(nb::init<int>(), "precision"_a = 2)
        .def("add_subject", &ClipperD::AddSubject)
        .def("add_open_subject", &ClipperD::AddOpenSubject)
        .def("add_clip", &ClipperD::AddClip)
        .def("execute",
             nb::overload_cast<ClipType, FillRule, PathsD&, PathsD&>(&ClipperD::Execute))
        .def("clear", &ClipperD::Clear)
        .def("preserve_collinear", &ClipperD::PreserveCollinear)
        .def("reverse_solution", &ClipperD::ReverseSolution);

    // ClipperOffset
    nb::class_<ClipperOffset>(m, "ClipperOffset")
        .def(nb::init<double, double, bool>(),
             "miter_limit"_a = 2.0,
             "arc_tolerance"_a = 0.0,
             "reverse_solution"_a = false)
        .def("add_path", &ClipperOffset::AddPath)
        .def("add_paths", &ClipperOffset::AddPaths)
        .def("execute", 
             nb::overload_cast<double, Paths64&>(&ClipperOffset::Execute))
        .def("clear", &ClipperOffset::Clear);

    // Utility functions
    m.def("version", &Version, "Get Clipper2 version");
    
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