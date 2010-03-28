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
