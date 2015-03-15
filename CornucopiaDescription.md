# Overview #

Cornucopia is a library for turning mouse or tablet input into a smooth curve.

The goal is to produce curves that are both simpler and more faithful to the user sketch, compared to traditional spline fitting methods.  The output is in the form of a curve with piecewise linear curvature (a clothoid spline), but routines are included to convert it to a bezier spline.

Cornucopia is an implementation of the algorithm described in my paper:

Ilya Baran, Jaakko Lehtinen, Jovan Popović "Sketching Clothoid Splines Using Shortest Paths", Eurographics 2010.
`[`[.pdf](http://people.csail.mit.edu/ibaran/curvesAndSupp.pdf)`]`

The main idea of the algorithm is to approximate all possible (reasonable) subsegments of the user's sketch with clothoids, use the A<sup>*</sup> algorithm to choose a subsequence of these clothoids to cover the curve, and use constrained nonlinear optimization to merge them into a single curve, while approximating the sketch as closely as possible.  The trick is doing all of this quickly and this implementation is almost 10x faster than the one timed for the paper, thanks to careful tuning and Eigen's speed.

<img src='http://wiki.cornucopia-lib.googlecode.com/hg/CornucopiaScreenshot.png' width='80%'></img>

The above screenshot shows DemoUI's debug window.  The dashed line is the original sketch, the colored spline is the fitted curve.  Clothoids are blue, circular arcs are green.

# Requirements #

Cornucopia is cross-platform (tested on Linux and Windows, with GCC 4 and MSVC 2008 and 2010).

The library only depends on [Eigen 3](http://eigen.tuxfamily.org).  The DemoUI debugging and demo app also requires Qt 4.6.

# Building #

The meta-build system is CMake.  For CMake's full documentation, see their [website](http://www.cmake.org/cmake/help/documentation.html).

The standard CMake instructions apply.  For example if under Linux, you installed Cornucopia to `~/Code/cornucopia-lib` and made a build directory at `~/Code/cornucopia`, you would run the following commands to build:
```
ibaran@ubuntu:~/Code/cornubuild$ cmake ../cornucopia-lib/
ibaran@ubuntu:~/Code/cornubuild$ make
ibaran@ubuntu:~/Code/cornubuild$ DemoUI/DemoUI
```
If CMake can find Eigen and Qt, all should be well.  Be sure to build in Release mode--without optimization, Eigen is extremely slow.

# Using #

DemoUI is fairly self-explanatory--sketch in the main window, see debugging output from the last sketch in the debug window, pan and zoom with the mouse wheel.  Oversketching only considers the currently selected curve(s) (shown in red).

Cornucopia can be invoked in two ways, using the standard API, and a simplified one, which doesn't use Eigen, my smart pointers, or anything other than STL vectors.

To use the simple API, include Cornucopia/SimpleAPI.h.
All you need to do is create a vector of 2D coordinates of mouse samples, and a Parameters object, and Cornucopia will return a vector of lines, arcs, and clothoids.  Another function call converts this vector into a Bezier spline.
Currently, the simple API does not support oversketching.

The file Test/EndToEndTest.cpp is a simple example of how to use Cornucopia.  The algorithm is controlled using the Parameters object, defined in Cornucopia/Parameters.h.  They will be documented at some point.

# Status #

Cornucopia is feature-complete and fairly robust, but is missing some documentation.  I also expect some small changes to the API.  Several algorithms (most notably, corner detection) can also use improvement (see ToDo).

Currently, Cornucopia is single-threaded and not thread-safe (statics are used for some initialization).  I expect that after the first complete fit run, it is thread-safe, but I have not tested this.

I am also interested in feedback regarding Cornucopia--send me an email if you find a use for it.

# Licensing #

All of the code is licensed under the GNU GPL 3.  If you would like to use it under a different license, email me and I will likely grant an exemption (possibly, for a small amount of money, if you're a company).  If you use the library for research, please cite the above paper.