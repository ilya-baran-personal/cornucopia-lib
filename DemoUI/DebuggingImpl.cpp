/*--
    DebuggingImpl.cpp  

    This file is part of the Cornucopia curve sketching library.
    Copyright (C) 2010 Ilya Baran (baran37@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "DebuggingImpl.h"
#include "ScrollScene.h"
#include "SceneItem.h"
#include "Curve.h"

#include <ctime>
#include <cstdarg>
#include <cstdio>

using namespace std;
using namespace Eigen;

DebuggingImpl *DebuggingImpl::get()
{
    //see if an instance exists
    DebuggingImpl *cur = dynamic_cast<DebuggingImpl *>(Cornu::Debugging::get());
    if(cur)
        return cur;

    //create new instance
    cur = new DebuggingImpl();
    Cornu::Debugging::set(cur);
    return cur;
}

void DebuggingImpl::printf(const char *fmt, ...)
{
    const int sz = 5000; //we don't need terribly long debug strings
    char buffer[sz];

    va_list ap;
    va_start(ap, fmt);

    vsnprintf(buffer, sz, fmt, ap);

    va_end(ap);

    std::printf("%s\n", buffer);

    emit print(QString::fromAscii(buffer));
}

void DebuggingImpl::startTiming(const string &description)
{
    _startTimes[QString(description.c_str())] = clock();
}

void DebuggingImpl::elapsedTime(const string &description)
{
    QString desc(description.c_str());
    if(!_startTimes.contains(desc))
    {
        printf("Timing description not found: %s", description.c_str());

        return;
    }
    int diff = clock() - _startTimes[desc];
    printf("Timing: %20s  ---  %.3lf", description.c_str(), double(diff) / CLOCKS_PER_SEC);
}

double DebuggingImpl::getTimeElapsed(const string &description)
{
    QString desc(description.c_str());
    if(!_startTimes.contains(desc))
        return 0.;
    return double(clock() - _startTimes[desc]) / CLOCKS_PER_SEC;
}

void DebuggingImpl::clear(const std::string &groups)
{
    if(_scene)
        _scene->clearGroups(groups.c_str());
}

void DebuggingImpl::drawPoint(const Vector2d &pos, const Color &color, const std::string &group)
{
    if(_scene)
        _scene->addItem(new PointSceneItem(pos, group.c_str(), QColor::fromRgbF(color[0], color[1], color[2])));
}

void DebuggingImpl::drawLine(const Vector2d &p1, const Vector2d &p2, const Color &color, const std::string &group, double thickness, LineStyle style)
{
    if(_scene)
        _scene->addItem(new LineSceneItem(p1, p2, group.c_str(), _toQPen(color, thickness, style)));
}

void DebuggingImpl::drawCurve(Cornu::CurveConstPtr curve, const Color &color, const std::string &group, double thickness, LineStyle style)
{
    if(_scene)
        _scene->addItem(new CurveSceneItem(curve, group.c_str(), _toQPen(color, thickness, style)));
}

void DebuggingImpl::drawCurvatureField(Cornu::CurveConstPtr curve, const Color &color, const std::string &group, double thickness, LineStyle style)
{
    if(!_scene)
        return;

    const double stepSize = 5.;
    const double scale = 3000.;

    double step = curve->length() / (int(1 + curve->length() / stepSize));

    for(double x = 0; x < curve->length() + step * 0.5; x += step)
    {
        Vector2d pos, der, der2;
        curve->eval(x, &pos, &der, &der2);
        Vector2d norm(-der[1], der[0]);
        double length = norm.dot(der2) * scale;
        _scene->addItem(new LineSceneItem(pos, pos - norm * length, group.c_str(), _toQPen(color, thickness, style)));
    }
}

QPen DebuggingImpl::_toQPen(const Color &color, double thickness, LineStyle style)
{
    QPen out(QColor::fromRgbF(color[0], color[1], color[2]), thickness);

    switch(style)
    {
    case SOLID:
        break;
    case DASHED:
        out.setStyle(Qt::DashLine);
        break;
    case DOTTED:
        out.setStyle(Qt::DotLine);
        break;
    }

    return out;
}

#include "DebuggingImpl.moc"
