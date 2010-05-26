/*--
    MainView.cpp  

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

#include "MainView.h"
#include "Document.h"
#include "ScrollScene.h"
#include "SceneItem.h"
#include "Polyline.h"

#include <QMouseEvent>

using namespace std;
using namespace Eigen;

MainView::MainView(QWidget *parent, ParamWidget *paramWidget)
    : ScrollView(parent), _paramWidget(paramWidget), _pointsDrawn(0, Cornu::NOT_CIRCULAR)
{
    _document = new Document(this);
}

void MainView::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        _pointsDrawn.clear();

        _prevMousePos = e->pos();
        QPointF scenePt = viewToScene(_prevMousePos);
        _pointsDrawn.push_back(Vector2d(scenePt.x(), scenePt.y()));

        return;
    }
    ScrollView::mousePressEvent(e);
}

void MainView::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        scene()->clearGroups("currentlyDrawing");
        _document->curveDrawn(new Cornu::Polyline(_pointsDrawn));
        return;
    }
    ScrollView::mouseReleaseEvent(e);
}

void MainView::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        _prevMousePos = e->pos();
        QPointF scenePt = viewToScene(_prevMousePos);
        _pointsDrawn.push_back(Vector2d(scenePt.x(), scenePt.y()));

        scene()->clearGroups("currentlyDrawing");
        scene()->addItem(new CurveSceneItem(new Cornu::Polyline(_pointsDrawn), "currentlyDrawing"));

        return;
    }
    ScrollView::mouseMoveEvent(e);
}


#include "MainView.moc"
