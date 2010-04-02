/*--
    DebuggingImpl.cpp  

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

#include "DebuggingImpl.h"
#include "ScrollScene.h"
#include "SceneItem.h"

#include <ctime>
#include <cstdarg>

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
    const int sz = 200; //we don't need terribly long debug strings
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
    printf("Timing: %20s   ---   %.3lf", description.c_str(), double(diff) / CLOCKS_PER_SEC);
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

void DebuggingImpl::drawLine(const Vector2d &p1, const Vector2d &p2, const Color &color, const std::string &group, double thickness)
{
    if(_scene)
        _scene->addItem(new LineSceneItem(p1, p2, group.c_str(), QPen(QColor::fromRgbF(color[0], color[1], color[2]), thickness)));
}

void DebuggingImpl::drawCurve(Cornu::CurveConstPtr curve, const Color &color, const std::string &group, double thickness)
{
    if(_scene)
        _scene->addItem(new CurveSceneItem(curve, group.c_str(), QPen(QColor::fromRgbF(color[0], color[1], color[2]), thickness)));
}


#include "DebuggingImpl.moc"
