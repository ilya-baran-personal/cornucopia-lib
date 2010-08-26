/*--
    DebugWindow.cpp  

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
