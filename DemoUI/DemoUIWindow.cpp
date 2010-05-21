/*--
    DemoUIWindow.cpp  

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

#include "DemoUIWindow.h"
#include "ui_DebugWindow.h"
#include "DebuggingImpl.h"
#include "ParamWidget.h"
#include "MainView.h"

#include "libTest.h"

#include <QScrollArea>
#include <QMenuBar>
#include <QMenu>

using namespace std;
using namespace Eigen;

DemoUIWindow::DemoUIWindow()
{
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QMenu *viewMenu = menuBar()->addMenu("&View");

    QAction *quit = new QAction("&Quit", this);
    quit->setShortcut(QKeySequence("Ctrl+Q"));
    fileMenu->addAction(quit);
    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));

    //Initialize the main window
    MainView *mainView = new MainView(this);
    setCentralWidget(mainView);
    ParamWidget *paramWidget = new ParamWidget(this);
    addDockWidget(Qt::RightDockWidgetArea, paramWidget);

    //Initialize the debugging window
    _debugWindow = new QMainWindow();
    Ui::DebugWindow debugWindowUi;
    debugWindowUi.setupUi(_debugWindow);

    connect(debugWindowUi.actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    QAction *showDebugWindow = new QAction("Show &debug window", this);
    viewMenu->addAction(showDebugWindow);

    connect(showDebugWindow, SIGNAL(triggered()), _debugWindow, SLOT(show()));
    connect(DebuggingImpl::get(), SIGNAL(print(QString)), debugWindowUi.debugText, SLOT(appendPlainText(QString)));

    ScrollScene *scene = debugWindowUi.debugView->scene(); //the ScrollView created it
    DebuggingImpl::get()->setScene(scene);
    debugWindowUi.groupSelWidget->setScene(scene);

    _debugWindow->show();

    Cornu::f(4);
}

DemoUIWindow::~DemoUIWindow()
{
    delete _debugWindow;
}
    

#include "DemoUIWindow.moc"
