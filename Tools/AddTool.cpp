#include "AddTool.h"
#include "ui_AddToolDialog.h"
#include "config.h"

#include <QDialog>
#include <QDebug>
#include <QFileInfo>

static QString cppTemplateFileName = QString(CORNUCOPIA_SOURCE_DIR) + "/Tools/cpp_template.txt";
static QString hTemplateFileName = QString(CORNUCOPIA_SOURCE_DIR) + "/Tools/h_template.txt";

AddTool::AddTool(QWidget *parent)
: Tool(parent)
{
}

QString AddTool::getName() const
{
    return QString("Add...");
}

void AddTool::execute()
{
    QDialog *dialog = new QDialog(parentWidget());
    Ui::AddToolDialog dialogUi;
    dialogUi.setupUi(dialog);
    if(!dialog->exec())
        return;

    QString projectName;
    if(dialogUi.inCornucopia->isChecked())
        projectName = QString("Cornucopia");
    if(dialogUi.inDemoUI->isChecked())
        projectName = QString("DemoUI");
    if(dialogUi.inTools->isChecked())
        projectName = QString("Tool");

    _add(dialogUi.fileName->text(), dialogUi.header->isChecked(), dialogUi.source->isChecked(), projectName);
}

void AddTool::_add(QString fileName, bool header, bool source, QString project)
{
    qDebug() << "Adding:" << fileName << "in project" << project << "Header =" << header << "Source =" << source;

    QString headerFileName = QString(CORNUCOPIA_SOURCE_DIR) + "/" + project + "/" + fileName + ".h";
    QString sourceFileName = QString(CORNUCOPIA_SOURCE_DIR) + "/" + project + "/" + fileName + ".cpp";

    if(header && QFileInfo(headerFileName).exists())
    {
        toolError(QString("File: ") + headerFileName + " already exists.");
        return;
    }
    if(source && QFileInfo(sourceFileName).exists())
    {
        toolError(QString("File: ") + sourceFileName + " already exists.");
        return;
    }

    QList<QPair<QRegExp, QString> > mapping;

    mapping.append(qMakePair(QRegExp("@FILENAME@"), fileName));
    mapping.append(qMakePair(QRegExp("@CAPSFILENAME@"), fileName.toUpper()));

    if(header)
        if(!_addOneFile(hTemplateFileName, headerFileName, mapping))
            return;

    if(source)
        _addOneFile(cppTemplateFileName, sourceFileName, mapping);
}

bool AddTool::_addOneFile(QString templateName, QString targetName,
                          const QList<QPair<QRegExp, QString> > &mapping)
{
    qDebug() << "Adding file:" << targetName << "from" << templateName;

    QFile templateFile(templateName);
    QFile targetFile(targetName);

    if(!targetFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        toolError(QString("Could not open file: ") + targetName + " for write.");
        return false;
    }

    if(!templateFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        toolError(QString("Could not open file: ") + templateName + " for read.");
        return false;
    }

    QTextStream fromStream(&templateFile);
    QTextStream toStream(&targetFile);

    transformStream(fromStream, toStream, mapping);

    return true;
}

#include "AddTool.moc"
