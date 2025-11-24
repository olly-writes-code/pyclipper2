# pyclipper2

Python bindings for [Clipper2](https://github.com/AngusJohnson/Clipper2/tree/main)

### Why does this exists?
Clipper2 is now alive and kicking. It should be preferred vs the old Clipper library (see docs [here](https://www.angusj.com/clipper2/Docs/Overview.htm)).

While one could leverage [Shapely](https://shapely.readthedocs.io/en/stable/) for some 2D line and shape operations, it's performance is limited. Using this library you can get a ~80% performance improvement for operations like union and difference (for small polygons).

### ⚠️ Warning! 

This package is very very early. I have only published wheels for python 3.13 and ARM Macs


