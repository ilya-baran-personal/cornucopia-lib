/*--
    ScrollView.cpp  

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
: QAbstractScrollArea(parent), _zoom(1.), _updating(0)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    _scene = new ScrollScene(this);
    connect(_scene, SIGNAL(sceneChanged()), this, SLOT(update()));
}

void ScrollView::scrollContentsBy(int dx, int dy)
{
    if(_updating)
        return;

    _offset += QPointF(dx, dy);

    update();
    viewport()->repaint();
}

void ScrollView::paintEvent(QPaintEvent *)
{
    QPainter p(viewport());
    p.setRenderHint(QPainter::Antialiasing);

    _scene->draw(&p, QTransform(_zoom, 0, 0, _zoom, _offset.x(), _offset.y()));
}

void ScrollView::resizeEvent(QResizeEvent *)
{
    update();
}

void ScrollView::update()
{
    ++_updating;

    QRect viewRect = viewport()->geometry();
    
    QTransform scene2view(_zoom, 0, 0, _zoom, _offset.x(), _offset.y());
    QRectF sceneRectF = _scene->rect();
    QRect sceneRect = scene2view.mapRect(sceneRectF).toAlignedRect(); //scene rect in view coordinates

    if(sceneRectF.isEmpty())
    {
        horizontalScrollBar()->setRange(0, 0);
        verticalScrollBar()->setRange(0, 0);
        viewport()->update();
        --_updating;
        return;
    }

    const int adj = 5; //pixels around the scene
    sceneRect.adjust(-adj, -adj, adj, adj);

    horizontalScrollBar()->setPageStep(viewRect.width());
    verticalScrollBar()->setPageStep(viewRect.height());

    int minX = min(0, sceneRect.left());
    int maxX = max(0, sceneRect.right() - viewRect.width());
    horizontalScrollBar()->setRange(minX, maxX);
    
    int minY = min(0, sceneRect.top());
    int maxY = max(0, sceneRect.bottom() - viewRect.height());
    verticalScrollBar()->setRange(minY, maxY);

    horizontalScrollBar()->setValue(0);
    verticalScrollBar()->setValue(0);

    viewport()->update();

    --_updating;
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

    _offset += QPointF(dx, dy);
    update();
    viewport()->repaint();
}

void ScrollView::wheelEvent(QWheelEvent *e)
{
    const double minZoom = 1e-4;
    const double maxZoom = 1e4;
    const double zoomFactor = 1.002;
    double oldZoom = _zoom;
    _zoom *= pow(zoomFactor, e->delta());
    _zoom = max(minZoom, min(maxZoom, _zoom));

    //scale the offset too
    QPointF mousePos(e->pos());
    _offset = mousePos + (_offset - mousePos) * (_zoom / oldZoom);

    update();
    viewport()->repaint();
}

void ScrollView::resetView()
{
    _offset = QPointF();
    _zoom = 1.;

    update();
    viewport()->repaint();
}

#include "ScrollView.moc"
