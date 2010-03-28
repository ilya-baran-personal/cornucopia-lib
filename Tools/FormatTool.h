#ifndef FORMATTOOL_H_INCLUDED
#define FORMATTOOL_H_INCLUDED

#include "Tool.h"

class FormatTool : public Tool
{
    Q_OBJECT
public:
    FormatTool(QWidget *parent) : Tool(parent) {}

    //override
    QString getName() const { return QString("Format files"); }

public slots:
    //override
    void execute();

private:
    void _formatFile(QString fileName);
};


#endif //FORMATTOOL_H_INCLUDED
