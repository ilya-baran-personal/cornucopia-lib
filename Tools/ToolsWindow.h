#ifndef TOOLSWINDOW_H_INCLUDED
#define TOOLSWINDOW_H_INCLUDED

#include <QMainWindow>

class Tool;

class ToolsWindow : public QMainWindow
{
    Q_OBJECT
public:
    ToolsWindow();

private:
    void addTool(Tool *tool);
};

#endif //TOOLSWINDOW_H_INCLUDED
