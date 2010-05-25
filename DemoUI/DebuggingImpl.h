/*--
    DebuggingImpl.h  

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

#ifndef DEBUGGINGIMPL_H_INCLUDED
#define DEBUGGINGIMPL_H_INCLUDED

#include "defs.h"
#include "Debugging.h"

#include <QObject>
#include <QHash>
#include <QPen>

class ScrollScene;

class DebuggingImpl : public QObject, public Cornu::Debugging
{
    Q_OBJECT
public:
    static DebuggingImpl *get();

    //overrides
    void printf(const char *fmt, ...);

    void startTiming(const std::string &description);
    void elapsedTime(const std::string &description);
    double getTimeElapsed(const std::string &description);

    void clear(const std::string &groups = "");
    void drawPoint(const Vector2d &pos, const Color &color, const std::string &group = "");
    void drawLine(const Vector2d &p1, const Vector2d &p2, const Color &color, const std::string &group = "", double thickness = 1, LineStyle style = SOLID);
    void drawCurve(Cornu::CurveConstPtr curve, const Color &color, const std::string &group = "", double thickness = 1, LineStyle style = SOLID);

    //not overrides
    void setScene(ScrollScene *scene) { _scene = scene; }

signals:
    void print(QString);

private:
    DebuggingImpl() : _scene(NULL) {}

    static QPen _toQPen(const Color &color, double thickness, LineStyle style);

    ScrollScene *_scene;

    QHash<QString, int> _startTimes; 
};

#endif //DEBUGGINGIMPL_H_INCLUDED
