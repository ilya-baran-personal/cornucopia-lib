/*--
    SceneItem.h  

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

#ifndef SCENEITEM_H_INCLUDED
#define SCENEITEM_H_INCLUDED

#include "defs.h"
#include "smart_ptr.h"

#include <QBrush>
#include <QPen>
#include <QPainterPath>

#include <Eigen/Core>

class SceneItem : public Cornu::smart_base
{
public:
    SceneItem(QString group, QPen pen = QPen(), QBrush brush = QBrush())
        : _group(group), _pen(pen), _brush(brush) {}

    virtual void draw(QPainter *, const QTransform &) const = 0;
    virtual QRectF rect() const = 0;

    QString group() const { return _group; }

protected:
    QString _group;
    QPen _pen;
    QBrush _brush;
};

class PointSceneItem : public SceneItem
{
public:
    PointSceneItem(const Eigen::Vector2d &pos, QString group, QColor color);

    //overrides
    void draw(QPainter *p, const QTransform &) const;
    QRectF rect() const;

private:
    static const int radius = 3;
    QPointF _pos;
};

class LineSceneItem : public SceneItem
{
public:
    LineSceneItem(const Eigen::Vector2d &p1, const Eigen::Vector2d &p2, QString group, QPen pen = QPen(), QBrush brush = QBrush());

    //overrides
    void draw(QPainter *p, const QTransform &) const;
    QRectF rect() const;

private:
    QPointF _p1, _p2;
};

class CurveSceneItem : public SceneItem
{
public:
    CurveSceneItem(Cornu::CurveConstPtr curve, QString group, QPen pen = QPen(), QBrush brush = QBrush());

    //overrides
    void draw(QPainter *p, const QTransform &) const;
    QRectF rect() const { return _rect; }

private:
    Cornu::CurveConstPtr _curve;
    QRectF _rect;
    QPolygonF _curveTess;
};

#endif //SCENEITEM_H_INCLUDED
