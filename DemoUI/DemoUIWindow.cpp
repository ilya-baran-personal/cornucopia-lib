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

#include <QScrollArea>
#include <QMenuBar>
#include <QMenu>

using namespace std;
using namespace Eigen;

DemoUIWindow::DemoUIWindow()
{
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QMenu *viewMenu = menuBar()->addMenu("&View");

    //Initialize the debugging window
    _debugWindow = new QMainWindow();
    Ui::DebugWindow debugWindowUi;
    debugWindowUi.setupUi(_debugWindow);

    QAction *showDebugWindow = new QAction("Show debug window", this);
    viewMenu->addAction(showDebugWindow);

    connect(showDebugWindow, SIGNAL(triggered()), _debugWindow, SLOT(show()));
    connect(DebuggingImpl::get(), SIGNAL(print(QString)), debugWindowUi.debugText, SLOT(appendPlainText(QString)));

    DebuggingImpl::get()->setScene(debugWindowUi.debugView->scene());

    _debugWindow->show();
}

DemoUIWindow::~DemoUIWindow()
{
    delete _debugWindow;
}
    

#include "DemoUIWindow.moc"
