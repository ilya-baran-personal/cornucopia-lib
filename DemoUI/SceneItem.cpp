/*--
    SceneItem.cpp  

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

#include "SceneItem.h"
#include "Curve.h"
#include "Polyline.h"

#include <QPainter>

using namespace std;
using namespace Eigen;

PointSceneItem::PointSceneItem(const Vector2d &pos, QString group, QColor color)
: SceneItem(group, QPen(color), QBrush(color)), _pos(pos[0], pos[1])
{
}

void PointSceneItem::draw(QPainter *p, const QTransform &transform) const
{
    p->setPen(_pen);
    p->setBrush(_brush);
    p->drawEllipse(QRectF(transform.map(_pos) - QPointF(radius, radius), QSizeF(radius * 2, radius * 2)));
}

QRectF PointSceneItem::rect() const
{
    return QRectF(_pos - QPointF(radius, radius), QSizeF(radius * 2, radius * 2));
}

LineSceneItem::LineSceneItem(const Vector2d &p1, const Vector2d &p2, QString group, QPen pen, QBrush brush)
: SceneItem(group, pen, brush), _p1(p1[0], p1[1]), _p2(p2[0], p2[1])
{
}

void LineSceneItem::draw(QPainter *p, const QTransform &transform) const
{
    p->setPen(_pen);
    p->drawLine(transform.map(_p1), transform.map(_p2));
}

QRectF LineSceneItem::rect() const
{
    return QRectF(_p1, _p2).normalized();
}

CurveSceneItem::CurveSceneItem(Cornu::CurveConstPtr curve, QString group, QPen pen, QBrush brush)
: SceneItem(group, pen, brush), _curve(curve)
{
    Vector2d pt;

    //dynamically check if it's a polyline
    Cornu::PolylineConstPtr polyline = Cornu::dynamic_pointer_cast<const Cornu::Polyline>(curve);
    if(polyline)
    {
        for(int i = 0; i < (int)polyline->pts().size(); ++i)
        {
            pt = polyline->pts()[i];
            _curveTess.push_back(QPointF(pt[0], pt[1]));
        }
        if(polyline->isClosed())
        {
            pt = polyline->pts()[0];
            _curveTess.push_back(QPointF(pt[0], pt[1]));
        }
    }
    else
    {
        for(double s = 0; s < curve->length(); s += 5.) //skip pixels
        {
             pt = curve->pos(s);
            _curveTess.push_back(QPointF(pt[0], pt[1]));
        }
        pt = curve->endPos();
        _curveTess.push_back(QPointF(pt[0], pt[1]));
    }

    _rect = _curveTess.boundingRect();
}

void CurveSceneItem::draw(QPainter *p, const QTransform &transform) const
{
    p->setPen(_pen);
    p->drawPolyline(transform.map(_curveTess));
}
