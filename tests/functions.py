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
