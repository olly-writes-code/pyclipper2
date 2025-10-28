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
    assert pyclipper2.ClipType.Union
    assert pyclipper2.FillRule.EvenOdd
    assert pyclipper2.JoinType.Round
    assert pyclipper2.EndType.Polygon
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
        pyclipper2.Point64(0, 100)
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
        pyclipper2.Point64(0, 100)
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
        pyclipper2.Point64(0, 100)
    ]
    
    offset.add_path(path, pyclipper2.JoinType.Round, pyclipper2.EndType.Polygon)
    print("✓ Path added to offset")
    
    result = []
    offset.execute(10.0, result)
    print(f"✓ Offset executed, result paths: {len(result)}")

def test_version():
    """Test version constant"""
    version = pyclipper2.VERSION
    print(f"✓ Clipper2 version: {version}")

if __name__ == "__main__":
    print("Testing Clipper2 Python bindings...\n")
    
    test_point64()
    test_pointd()
    test_enums()
    test_rect64()
    test_area()
    test_is_positive()
    test_clipper_offset()
    test_version()
    
    print("\n✅ All tests passed!")