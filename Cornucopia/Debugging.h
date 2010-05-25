/*--
    Debugging.h  

    This file is part of the Cornucopia curve sketching library.
    Copyright (C) 2010 Ilya Baran (ibaran@mit.edu)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef DEBUGGING_H_INCLUDED
#define DEBUGGING_H_INCLUDED

#include "defs.h"
#include "smart_ptr.h"

#include <string>
#include <Eigen/Core>

NAMESPACE_Cornu

SMART_FORW_DECL(Curve);

//This class does nothing by default
class Debugging
{
public:
    typedef Eigen::Vector3d Color;
    typedef Eigen::Vector2d Vector2d;

    enum LineStyle
    {
        SOLID,
        DASHED,
        DOTTED
    };

    static Debugging *get() { return _currentDebugging; }

    virtual ~Debugging() {}

    virtual void printf(const char * /*fmt*/, ...) {}

    virtual void startTiming(const std::string &/*description*/) {}
    virtual void elapsedTime(const std::string &/*description*/) {} //prints the elapsed time
    virtual double getTimeElapsed(const std::string &/*description*/) { return 0.; } //in seconds

    virtual void clear(const std::string &/*groups*/ = "") {}

    virtual void drawPoint(const Vector2d &/*pos*/, const Color &/*color*/, const std::string &/*group*/ = "") {}
    virtual void drawLine(const Vector2d &/*p1*/, const Vector2d &/*p2*/, const Color &/*color*/, const std::string &/*group*/ = "", double /*thickness*/ = 1, LineStyle /*style*/ = SOLID) {}
    virtual void drawCurve(CurveConstPtr /*curve*/, const Color &/*color*/, const std::string &/*group*/ = "", double /*thickness*/ = 1, LineStyle /*style*/ = SOLID) {}

protected:
    Debugging() {}
    static void set(Debugging *debugging);

private:
    static Debugging *_currentDebugging;
};

END_NAMESPACE_Cornu

#endif //DEBUGGING_H_INCLUDED
