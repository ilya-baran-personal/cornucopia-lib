/*--
    main.cpp  

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

#include <QApplication>

#include "DemoUIWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //ofstream *strm = new ofstream("debug.log");
    //Debugging::setOutStream(*strm);

    DemoUIWindow *mw = new DemoUIWindow();
    mw->resize(1024, 640);
    mw->show();

    int out = app.exec();
    delete mw;
    return out;
}
