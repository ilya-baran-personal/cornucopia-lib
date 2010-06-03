/*--
    DebugWindow.cpp  

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

#include "DebugWindow.h"
#include "ui_DebugWindow.h"
#include "DebuggingImpl.h"
#include "ScrollScene.h"

using namespace std;
using namespace Eigen;

DebugWindow::DebugWindow()
{
    Ui::DebugWindow debugWindowUi;
    debugWindowUi.setupUi(this);

    connect(debugWindowUi.actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    connect(DebuggingImpl::get(), SIGNAL(print(QString)), debugWindowUi.debugText, SLOT(appendPlainText(QString)));

    _scene = debugWindowUi.debugView->scene(); //the ScrollView created it
    debugWindowUi.groupSelWidget->setScene(_scene);
}

void DebugWindow::showEvent(QShowEvent *)
{
    DebuggingImpl::get()->setScene(_scene);
}

void DebugWindow::hideEvent(QHideEvent *)
{
    _scene->clearGroups("");
    DebuggingImpl::get()->setScene(NULL);
}

#include "DebugWindow.moc"
