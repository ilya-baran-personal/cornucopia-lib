/*--
    ScrollView.h  

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

#ifndef SCROLLVIEW_H_INCLUDED
#define SCROLLVIEW_H_INCLUDED

#include "defs.h"

#include <QAbstractScrollArea>
#include <QVector2D>

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

protected:
    void scrollContentsBy(int dx, int dy);
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void wheelEvent(QWheelEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

protected slots:
    void updateScrollBars();
    void resetView();

private:
    void _pan(int dx, int dy);

    //to map from view to scene: add scroll bar values and divide by zoom
    double _zoom;
    QVector2D _offset;

    ScrollScene *_scene;
    QPoint _prevMousePos;
    int _updating;
};


#endif //SCROLLVIEW_H_INCLUDED
