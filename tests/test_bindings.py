import pyclipper2


def test_point64():
    """Test Point64 creation and attributes"""
    p = pyclipper2.Point64(100, 200)
    assert p.x == 100
    assert p.y == 200
    print(f"✓ Point64: {p}")


def test_pointd():
    """Test PointD creation and attributes"""
    p = pyclipper2.PointD(10.5, 20.7)
    assert p.x == 10.5
    assert p.y == 20.7
    print(f"✓ PointD: {p}")


def test_enums():
    """Test enum values"""
    assert pyclipper2.ClipType.UNION
    assert pyclipper2.ClipType.DIFFERENCE
    assert pyclipper2.ClipType.INTERSECTION
    assert pyclipper2.ClipType.XOR
    assert pyclipper2.FillRule.NON_ZERO
    assert pyclipper2.FillRule.POSITIVE
    assert pyclipper2.FillRule.NEGATIVE
    assert pyclipper2.FillRule.EVEN_ODD
    assert pyclipper2.EndType.POLYGON
    assert pyclipper2.EndType.JOINED
    assert pyclipper2.EndType.BUTT
    assert pyclipper2.EndType.SQUARE
    assert pyclipper2.EndType.ROUND
    assert pyclipper2.PathType.SUBJECT
    assert pyclipper2.PathType.CLIP
    assert pyclipper2.JoinWith.NO_JOIN
    assert pyclipper2.JoinWith.LEFT
    assert pyclipper2.JoinWith.RIGHT
    assert pyclipper2.PointInPolygonResult.IS_INSIDE
    assert pyclipper2.PointInPolygonResult.IS_OUTSIDE
    assert pyclipper2.PointInPolygonResult.IS_ON
    assert pyclipper2.JoinType.ROUND
    assert pyclipper2.JoinType.SQUARE
    assert pyclipper2.JoinType.BEVEL
    assert pyclipper2.JoinType.MITER
    print("✓ Enums work")


def test_rect64():
    """Test Rect64"""
    r = pyclipper2.Rect64(0, 0, 100, 100)
    assert r.left == 0
    assert r.right == 100
    print("✓ Rect64 works")


def test_area():
    """Test area calculation"""
    # Create a simple square path
    square = [
        pyclipper2.Point64(0, 0),
        pyclipper2.Point64(100, 0),
        pyclipper2.Point64(100, 100),
        pyclipper2.Point64(0, 100),
    ]
    area = pyclipper2.area(square)
    print(f"✓ Area of square: {area}")
    assert area != 0  # Should be 10000


def test_is_positive():
    """Test is_positive orientation"""
    # Clockwise square (should be negative in typical coordinate systems)
    square = [
        pyclipper2.Point64(0, 0),
        pyclipper2.Point64(100, 0),
        pyclipper2.Point64(100, 100),
        pyclipper2.Point64(0, 100),
    ]
    result = pyclipper2.is_positive(square)
    print(f"✓ is_positive: {result}")


def test_clipper_offset():
    """Test ClipperOffset"""
    offset = pyclipper2.ClipperOffset()
    print("✓ ClipperOffset created")

    # Create a simple path
    path = [
        pyclipper2.Point64(0, 0),
        pyclipper2.Point64(100, 0),
        pyclipper2.Point64(100, 100),
        pyclipper2.Point64(0, 100),
    ]

    offset.add_path(path, pyclipper2.JoinType.ROUND, pyclipper2.EndType.POLYGON)
    print("✓ Path added to offset")

    result = []
    offset.execute(10.0, result)
    print(f"✓ Offset executed, result paths: {len(result)}")


def test_version():
    """Test version constant"""
    version = pyclipper2.VERSION
    print(f"✓ Clipper2 version: {version}")


def test_point_in_polygon():
    """Test point_in_polygon function"""
    polygon = [
        pyclipper2.Point64(0, 0),
        pyclipper2.Point64(100, 0),
        pyclipper2.Point64(100, 100),
        pyclipper2.Point64(0, 100),
    ]
    inside_point = pyclipper2.Point64(50, 50)
    outside_point = pyclipper2.Point64(150, 150)

    inside_result = pyclipper2.point_in_polygon(inside_point, polygon)
    outside_result = pyclipper2.point_in_polygon(outside_point, polygon)

    print(f"✓ point_in_polygon (inside): {inside_result}")
    print(f"✓ point_in_polygon (outside): {outside_result}")
    assert inside_result == pyclipper2.PointInPolygonResult.IS_INSIDE
    assert outside_result == pyclipper2.PointInPolygonResult.IS_OUTSIDE


def test_path():
    input = [[1, 1], [2, 2]]

    ### create path
    path = pyclipper2.make_path(input)

    ### inflate path
    pyclipper2.inflate_paths(
        [path], 0.5, pyclipper2.JoinType.ROUND, pyclipper2.EndType.ROUND
    )
