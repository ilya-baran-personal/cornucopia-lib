#include "ToolsWindow.h"
#include "AddTool.h"
#include "FormatTool.h"

#include <QAction>
#include <QMenuBar>

ToolsWindow::ToolsWindow()
{
    addTool(new AddTool(this));
    addTool(new FormatTool(this));
}

void ToolsWindow::addTool(Tool *tool)
{
    QAction *action = new QAction(tool->getName(), this);
    connect(action, SIGNAL(triggered()), tool, SLOT(execute()));

    menuBar()->addAction(action);
}

#include "ToolsWindow.moc"
