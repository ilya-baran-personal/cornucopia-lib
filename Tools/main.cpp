/*--
    main.cpp  

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
    mw->resize(400, 100);
    mw->show();

    int out = app.exec();
    delete mw;
    return out;
}
