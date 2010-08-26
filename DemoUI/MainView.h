/*--
    MainView.h  

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

#ifndef MAINVIEW_H_INCLUDED
#define MAINVIEW_H_INCLUDED

#include "defs.h"
#include "ScrollView.h"
#include "VectorC.h"

class Document;
class ParamWidget;

class MainView : public ScrollView
{
    Q_OBJECT
public:
    MainView(QWidget *parent, ParamWidget *paramWidget);

    Document *document() const { return _document; }
    ParamWidget *paramWidget() const { return _paramWidget; }

public slots:
    void setDrawTool() { _tool = DrawTool; }
    void setSelectTool() { _tool = SelectTool; }
    void setImage();
    void clearImage();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

private:
    enum ToolType
    {
        DrawTool,
        SelectTool
    };

    Cornu::VectorC<Eigen::Vector2d> _pointsDrawn;
    Document *_document;
    ParamWidget *_paramWidget;
    ToolType _tool;
};


#endif //MAINVIEW_H_INCLUDED
