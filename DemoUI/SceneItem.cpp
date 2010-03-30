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

#include <QPainter>

using namespace std;
using namespace Eigen;

LineSceneItem::LineSceneItem(const Vector2d &p1, const Vector2d &p2, QString group, QPen pen, QBrush brush)
: SceneItem(group, pen, brush), _p1(p1[0], p1[1]), _p2(p2[0], p2[1])
{
}

void LineSceneItem::draw(QPainter *p) const
{
    p->setPen(_pen);
    p->setBrush(_brush);
    p->drawLine(_p1, _p2);
}

QRectF LineSceneItem::rect() const
{
    return QRectF(_p1, _p2).normalized();
}
