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
