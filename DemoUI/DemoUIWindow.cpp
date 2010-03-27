#include "DemoUIWindow.h"

#include <QScrollArea>
#include <QMenuBar>
#include <QMenu>

DemoUIWindow::DemoUIWindow()
{
    QMenu *fileMenu = menuBar()->addMenu("&File");
    QMenu *viewMenu = menuBar()->addMenu("&View");
}

#include "DemoUIWindow.moc"
