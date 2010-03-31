/*--
    ScrollView.cpp  

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

#include "ScrollView.h"
#include "ScrollScene.h"
#include "SceneItem.h"

#include <QPainter>
#include <QScrollBar>
#include <QWheelEvent>
#include <QMouseEvent>

using namespace std;
using namespace Eigen;

ScrollView::ScrollView(QWidget *parent)
: QAbstractScrollArea(parent), _zoom(2.), _updating(0)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    _scene = new ScrollScene(this);
    connect(_scene, SIGNAL(sceneChanged()), this, SLOT(updateScrollBars()));

    _scene->addItem(new LineSceneItem(Vector2d(10, 10), Vector2d(-370, 530), "Line"));
}

void ScrollView::scrollContentsBy(int dx, int dy)
{
    updateScrollBars();
    viewport()->repaint();
}

void ScrollView::paintEvent(QPaintEvent *)
{
    QPainter p(viewport());
    int curX = horizontalScrollBar()->value();
    int curY = verticalScrollBar()->value();
    p.setWorldTransform(QTransform(_zoom, 0, 0, _zoom, -curX, -curY));
    p.setRenderHint(QPainter::Antialiasing);

    _scene->draw(&p);
}

void ScrollView::resizeEvent(QResizeEvent *)
{
    updateScrollBars();
}

void ScrollView::updateScrollBars()
{
    ++_updating;
    setUpdatesEnabled(false);

    QRect viewRect = viewport()->geometry();
    QRect sceneRect = QTransform::fromScale(_zoom, _zoom).mapRect(_scene->rect() ).toAlignedRect(); //scene rect in view coordinates
    const int adj = 5; //pixels around the scene
    sceneRect.adjust(-adj, -adj, adj, adj);

    int curX = horizontalScrollBar()->value();
    int curY = verticalScrollBar()->value();

    sceneRect |= viewRect.translated(curX, curY); //the scene always includes the view

    horizontalScrollBar()->setPageStep(viewRect.width());
    verticalScrollBar()->setPageStep(viewRect.height());

    int minX = sceneRect.left() - viewRect.left(), maxX = sceneRect.right() - viewRect.right();
    horizontalScrollBar()->setRange(minX, maxX);
    int minY = sceneRect.top() - viewRect.top(), maxY = sceneRect.bottom() - viewRect.bottom();
    verticalScrollBar()->setRange(minY, maxY);
    if(--_updating <= 0)
        setUpdatesEnabled(true);
}

void ScrollView::mousePressEvent(QMouseEvent *e)
{
    if(!(e->buttons() & Qt::MidButton))
        return;

    _prevMousePos = e->pos();
    setCursor(Qt::ClosedHandCursor);
}

void ScrollView::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() != Qt::MidButton)
        return;
    
    unsetCursor();
}

void ScrollView::mouseMoveEvent(QMouseEvent *e)
{
    if(!(e->buttons() & Qt::MidButton))
        return;
    
    int dx = e->x() - _prevMousePos.x();
    int dy = e->y() - _prevMousePos.y();
    _prevMousePos = e->pos();

    _pan(dx, dy);
    viewport()->repaint();
}

void ScrollView::_pan(int dx, int dy)
{
    ++_updating;
    setUpdatesEnabled(false);

    QScrollBar *h = horizontalScrollBar(), *v = verticalScrollBar();

    int x = h->value(), y = v->value();
    x -= dx; y -= dy;
    h->setRange(min(h->minimum(), x), max(h->maximum(), x));
    h->setValue(x);
    v->setRange(min(v->minimum(), y), max(v->maximum(), y));
    v->setValue(y);
    if(--_updating <= 0)
        setUpdatesEnabled(true);
}

void ScrollView::wheelEvent(QWheelEvent *e)
{
    const double minZoom = 1e-4;
    const double maxZoom = 1e4;
    const double zoomFactor = 1.002;
    double oldZoom = _zoom;
    _zoom *= pow(zoomFactor, e->delta());
    _zoom = max(minZoom, min(maxZoom, _zoom));

    //scale the slider values too
    int curX = -horizontalScrollBar()->value();
    int curY = -verticalScrollBar()->value();
    curX -= e->x();
    curY -= e->y();

    int newX = (int)floor(0.5 + curX * (_zoom / oldZoom));
    int newY = (int)floor(0.5 + curY * (_zoom / oldZoom));

    _pan(newX - curX, newY - curY);

    updateScrollBars();
    viewport()->repaint();
}

void ScrollView::resetView()
{
    int curX = -horizontalScrollBar()->value();
    int curY = -verticalScrollBar()->value();

    _zoom = 1.;
    _pan(-curX, -curY);
    viewport()->repaint();
}

#include "ScrollView.moc"
