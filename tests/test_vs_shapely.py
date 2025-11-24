import math

import pytest
from shapely import Point, Polygon, unary_union

import pyclipper2

SCALE_FACTOR = 1000000  # Convert float to int64 for Clipper


def shapely_to_path64(poly):
    """Convert Shapely polygon to Path64 (list of Point64)"""
    if poly.is_empty:
        return []
    coords = list(poly.exterior.coords)[:-1]  # Remove duplicate last point
    # Create Point64 objects
    return [
        pyclipper2.Point64(int(x * SCALE_FACTOR), int(y * SCALE_FACTOR))
        for x, y in coords
    ]


def paths64_to_shapely(paths):
    """Convert Paths64 back to Shapely polygon"""
    if not paths:
        return Polygon()

    polygons = []
    for path in paths:
        coords = [(float(p.x) / SCALE_FACTOR, float(p.y) / SCALE_FACTOR) for p in path]
        if len(coords) >= 3:
            polygons.append(Polygon(coords))

    if len(polygons) == 1:
        return polygons[0]
    elif len(polygons) > 1:
        return unary_union(polygons)
    else:
        return Polygon()


def test_high_level_functions():
    """Test the high-level boolean operation functions"""
    print("\n=== TESTING HIGH-LEVEL FUNCTIONS ===")

    # Create test polygons
    square1 = Polygon([(0, 0), (10, 0), (10, 10), (0, 10)])
    square2 = Polygon([(5, 5), (15, 5), (15, 15), (5, 15)])
    circle = Point(5, 5).buffer(3)

    paths1 = [shapely_to_path64(square1)]
    paths2 = [shapely_to_path64(square2)]
    paths3 = [shapely_to_path64(circle)]

    # Test union
    result = pyclipper2.union(paths1 + paths2, pyclipper2.FillRule.NON_ZERO)
    result_poly = paths64_to_shapely(result)
    assert result_poly.area == square1.union(square2).area

    # Test difference
    result = pyclipper2.difference(paths1, paths2, pyclipper2.FillRule.NON_ZERO)
    result_poly = paths64_to_shapely(result)
    assert result_poly.area == square1.difference(square2).area

    # Test intersection
    result = pyclipper2.intersection(paths1, paths3, pyclipper2.FillRule.NON_ZERO)
    result_poly = paths64_to_shapely(result)
    assert math.isclose(
        result_poly.area, square1.intersection(circle).area, rel_tol=1e-5
    )

    # Test XOR
    result = pyclipper2.xor_(paths1, paths2, pyclipper2.FillRule.NON_ZERO)
    result_poly = paths64_to_shapely(result)
    assert result_poly.area == square1.symmetric_difference(square2).area


def test_path_point_inflate_intersection():
    input_point = [1, 1]
    input_point_scaled = [
        [input_point[0] * SCALE_FACTOR, input_point[0] * SCALE_FACTOR]
    ]
    radius = 0.5
    point_path = pyclipper2.make_path(input_point_scaled)
    with_buffer = pyclipper2.inflate_paths(
        [point_path],
        radius * SCALE_FACTOR,
        pyclipper2.JoinType.ROUND,
        pyclipper2.EndType.ROUND,
    )
    square = Polygon([(0, 0), (10, 0), (10, 10), (0, 10)])
    square_path = [shapely_to_path64(square)]
    output = pyclipper2.intersection(
        with_buffer, square_path, pyclipper2.FillRule.NON_ZERO
    )
    pc_area = pyclipper2.area(output[0]) / (SCALE_FACTOR**2)

    ### compare vs shapely
    sh_point = Point(input_point).buffer(radius)
    intersection = square.intersection(sh_point)
    sh_area = intersection.area
    assert pc_area == pytest.approx(sh_area, rel=0.01)
