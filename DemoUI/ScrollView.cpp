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

using namespace std;
using namespace Eigen;

ScrollView::ScrollView(QWidget *parent)
: QAbstractScrollArea(parent), _zoom(2.)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    _scene = new ScrollScene(this);
    connect(_scene, SIGNAL(sceneChanged()), this, SLOT(updateScrollBars()));

    _scene->addItem(new LineSceneItem(Vector2d(10, 10), Vector2d(-370, 530), "Line"));
}

void ScrollView::scrollContentsBy(int dx, int dy)
{
    updateScrollBars(true);
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
    updateScrollBars(true);
}

void ScrollView::updateScrollBars(bool scrolling)
{
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
}

#include "ScrollView.moc"
