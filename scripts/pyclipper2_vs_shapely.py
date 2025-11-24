"""
RESULTS

Shapely Intersection: 1.0634839159902185
Shapely Difference: 1.0895179999060929
Pyclipper2 Intersection: 0.1792892919620499
Pyclipper2 Difference: 0.19169729098211974

Pyclipper2 has ~80% less runtime!

"""

import time

from shapely import Polygon, unary_union

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


def run():
    square1 = Polygon([(0, 0), (10, 0), (10, 10), (0, 10)])
    square2 = Polygon([(5, 5), (15, 5), (15, 15), (5, 15)])

    ### SHAPELY
    ### Intersection
    start_time = time.perf_counter()
    for _ in range(100000):
        square1.intersection(square2)
    shapely_int_time = time.perf_counter() - start_time
    print(f"Shapely Intersection: {shapely_int_time}")

    ### Difference
    start_time = time.perf_counter()
    for _ in range(100000):
        square2.difference(square1)
    shapely_diff_time = time.perf_counter() - start_time
    print(f"Shapely Difference: {shapely_diff_time}")

    ### Pyclipper2

    square1_pyclipper = [shapely_to_path64(square1)]
    square2_pyclipper = [shapely_to_path64(square2)]

    start_time = time.perf_counter()
    for _ in range(100000):
        pyclipper2.intersection(
            square1_pyclipper, square2_pyclipper, pyclipper2.FillRule.NON_ZERO
        )
    shapely_int_time = time.perf_counter() - start_time
    print(f"Pyclipper2 Intersection: {shapely_int_time}")

    start_time = time.perf_counter()
    for _ in range(100000):
        pyclipper2.difference(
            square2_pyclipper, square1_pyclipper, pyclipper2.FillRule.NON_ZERO
        )
    shapely_int_time = time.perf_counter() - start_time
    print(f"Pyclipper2 Difference: {shapely_int_time}")


if __name__ == "__main__":
    run()
