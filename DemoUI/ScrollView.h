/*--
    ScrollView.h  

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

#ifndef CORNUCOPIA_SCROLLVIEW_H_INCLUDED
#define CORNUCOPIA_SCROLLVIEW_H_INCLUDED

#include "defs.h"

#include <QAbstractScrollArea>
#include <QPointF>

class ScrollScene;
class QPaintEvent;
class QResizeEvent;
class QWheelEvent;
class QMouseEvent;

class ScrollView : public QAbstractScrollArea
{
    Q_OBJECT
public:
    ScrollView(QWidget *parent = NULL);

    ScrollScene *scene() const { return _scene; }

    QPointF sceneToView(const QPointF &scene) const { return scene * _zoom + _offset; }
    QPointF viewToScene(const QPointF &view) const { return (view - _offset) * (1. / _zoom); }
    double sceneToViewZoom() const { return _zoom; }

protected:
    QPoint _prevMousePos;

    void scrollContentsBy(int dx, int dy);
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void wheelEvent(QWheelEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

protected slots:
    void update();
    void resetView();

private:
    //to map from scene to view: multiply by zoom and add offset
    double _zoom;
    QPointF _offset;

    ScrollScene *_scene;
    int _updating;
};


#endif //CORNUCOPIA_SCROLLVIEW_H_INCLUDED
