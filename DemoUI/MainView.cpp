/*--
    MainView.cpp  

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

#include "MainView.h"
#include "Document.h"
#include "ScrollScene.h"
#include "SceneItem.h"
#include "Polyline.h"

#include <QMouseEvent>
#include <QFileDialog>

using namespace std;
using namespace Eigen;

MainView::MainView(QWidget *parent, ParamWidget *paramWidget)
    : ScrollView(parent), _paramWidget(paramWidget), _pointsDrawn(0, Cornu::NOT_CIRCULAR), _tool(DrawTool)
{
    _document = new Document(this);
}

void MainView::mousePressEvent(QMouseEvent *e)
{
    if(_tool == DrawTool)
    {
        if(e->buttons() & Qt::LeftButton)
        {
            _pointsDrawn.clear();

            _prevMousePos = e->pos();
            QPointF scenePt = viewToScene(_prevMousePos);
            _pointsDrawn.push_back(Vector2d(scenePt.x(), scenePt.y()));

            return;
        }
    }
    else if(_tool = SelectTool)
    {
    }

    ScrollView::mousePressEvent(e);
}

void MainView::mouseReleaseEvent(QMouseEvent *e)
{
    if(_tool == DrawTool)
    {
        if(e->button() == Qt::LeftButton)
        {
            scene()->clearGroups("currentlyDrawing");
            if(_pointsDrawn.size() > 1)
                _document->curveDrawn(new Cornu::Polyline(_pointsDrawn));
            return;
        }
    }
    else if(_tool = SelectTool)
    {
        QPointF scenePt = viewToScene(e->pos());

        _document->selectAt(Vector2d(scenePt.x(), scenePt.y()), e->modifiers() & Qt::SHIFT, 15. / sceneToViewZoom());
    }
    ScrollView::mouseReleaseEvent(e);
}

void MainView::mouseMoveEvent(QMouseEvent *e)
{
    if(_tool == DrawTool)
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
    }
    else if(_tool = SelectTool)
    {
    }
    ScrollView::mouseMoveEvent(e);
}

void MainView::clearImage()
{
    scene()->clearGroups("Background Image");
}

void MainView::setImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Choose the image file",
                    "",
                    "Image files (*.png *.jpg)");

    if(fileName.isEmpty())
        return;

    QImage image(fileName);

    if(image.isNull())
        return;

    scene()->clearGroups("Background Image");
    scene()->addItem(new ImageSceneItem(image, "Background Image"));
}

#include "MainView.moc"
