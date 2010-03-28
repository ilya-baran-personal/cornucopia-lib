#ifndef ADDTOOL_H_INCLUDED
#define ADDTOOL_H_INCLUDED

#include "Tool.h"

class AddTool : public Tool
{
    Q_OBJECT
public:
    AddTool(QWidget *parent);

    //override
    QString getName() const;

public slots:
    //override
    void execute();

private:
    void _add(QString fileName, bool header, bool source, QString project);
    bool _addOneFile(QString templateName, QString targetName,
                     const QList<QPair<QRegExp, QString> >& mapping); //returns false on failure
};

#endif //ADDTOOL_H_INCLUDED
