/*--
    DemoUIWindow.cpp  

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

#include "DemoUIWindow.h"
#include "ui_DemoUIWindow.h"
#include "ParamWidget.h"
#include "MainView.h"
#include "Document.h"
#include "DebugWindow.h"

#include "libTest.h"

#include <QScrollArea>
#include <QActionGroup>
#include <QMenuBar>
#include <QMenu>

using namespace std;
using namespace Eigen;

DemoUIWindow::DemoUIWindow()
{
    //Initialize the main window
    Ui::DemoUIWindow ui;
    ui.setupUi(this);

    ParamWidget *paramWidget = new ParamWidget(this);
    MainView *mainView = new MainView(this, paramWidget);
    setCentralWidget(mainView);
    addDockWidget(Qt::RightDockWidgetArea, paramWidget);
    
    connect(paramWidget, SIGNAL(rerunClicked()), mainView->document(), SLOT(refitSelected()));

    _debugWindow = new DebugWindow();

    QActionGroup *toolGroup = new QActionGroup(this);
    toolGroup->addAction(ui.actionDraw_Tool);
    toolGroup->addAction(ui.actionSelect_Tool);
    ui.actionDraw_Tool->setChecked(true);

    //menus
    connect(ui.action_Quit, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui.action_Reset_View, SIGNAL(triggered()), mainView, SLOT(resetView()));
    connect(ui.action_New, SIGNAL(triggered()), mainView->document(), SLOT(deleteAll()));
    connect(ui.actionOpen, SIGNAL(triggered()), mainView->document(), SLOT(open()));
    connect(ui.actionInsert, SIGNAL(triggered()), mainView->document(), SLOT(insert()));
    connect(ui.actionSave, SIGNAL(triggered()), mainView->document(), SLOT(save()));
    connect(ui.actionClear_Background_Image, SIGNAL(triggered()), mainView, SLOT(clearImage()));
    connect(ui.actionSet_Background_Image, SIGNAL(triggered()), mainView, SLOT(setImage()));
    connect(ui.actionShow_Debug_Window, SIGNAL(triggered()), _debugWindow, SLOT(show()));
    connect(ui.actionDelete, SIGNAL(triggered()), mainView->document(), SLOT(deleteItem()));
    connect(ui.actionSelect_All, SIGNAL(triggered()), mainView->document(), SLOT(selectAll()));

    connect(ui.actionDraw_Tool, SIGNAL(triggered()), mainView, SLOT(setDrawTool()));
    connect(ui.actionSelect_Tool, SIGNAL(triggered()), mainView, SLOT(setSelectTool()));

    _debugWindow->show();

    Cornu::f(4);
}

DemoUIWindow::~DemoUIWindow()
{
    delete _debugWindow;
}
    

#include "DemoUIWindow.moc"
