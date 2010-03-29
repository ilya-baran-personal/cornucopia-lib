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

#include "ToolsWindow.h"

#ifdef TMP_NOT__WIN32
#include <windows.h>

int WINAPI WinMain(
  __in  HINSTANCE hInstance,
  __in  HINSTANCE hPrevInstance,
  __in  LPSTR lpCmdLine,
  __in  int nCmdShow
)
{
    int argc = 0;
    char *empty = "";
    char **argv = &empty;
#else //Not WIN32
int main(int argc, char *argv[])
{
#endif //WIN32
    QApplication app(argc, argv);

    ToolsWindow *mw = new ToolsWindow();
    mw->show();

    int out = app.exec();
    delete mw;
    return out;
}
