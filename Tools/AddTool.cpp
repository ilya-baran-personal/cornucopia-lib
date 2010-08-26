/*--
    AddTool.cpp  

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

#include "AddTool.h"
#include "ui_AddToolDialog.h"
#include "FormatTool.h"
#include "config.h"

#include <QDialog>
#include <QDebug>
#include <QFileInfo>
#include <QProcess>

static const QString cppTemplateFileName = QString(CORNUCOPIA_SOURCE_DIR) + "/Tools/cpp_template.txt";
static const QString hTemplateFileName = QString(CORNUCOPIA_SOURCE_DIR) + "/Tools/h_template.txt";
static const QString testTemplateFileName = QString(CORNUCOPIA_SOURCE_DIR) + "/Tools/test_template.txt";
static const QString cmakeListsFileName = QString(CORNUCOPIA_SOURCE_DIR) + "/CMakeLists.txt";

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
        projectName = QString("Tools");
    if(dialogUi.inTest->isChecked())
        projectName = QString("Test");

    _add(dialogUi.fileName->text(), dialogUi.header->isChecked(),
         dialogUi.source->isChecked(), dialogUi.includeMoc->isChecked(), projectName);
}

void AddTool::_add(QString fileName, bool header, bool source, bool moc, QString project)
{
    if(project == "Test")
    {
        header = false;
        moc = false;
    }

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

    QString namespaceStr, endNamespaceStr;
    if(project == "Cornucopia") //add namespace defaults to cornucopia
    {
        namespaceStr = "NAMESPACE_Cornu\n";
        endNamespaceStr = "END_NAMESPACE_Cornu\n";
        moc = false; //Cornucopia has no Qt dependency
    }
    mapping.append(qMakePair(QRegExp("@NAMESPACE@"), namespaceStr));
    mapping.append(qMakePair(QRegExp("@ENDNAMESPACE@"), endNamespaceStr));

    QString includeMocString;
    if(moc)
    {
        includeMocString = QString("#include \"") + fileName + ".moc\"";
    }
    mapping.append(qMakePair(QRegExp("@MOCINCLUDE@"), includeMocString));

    if(header)
        if(!_addOneFile(hTemplateFileName, headerFileName, mapping))
            return;

    if(source)
    {
        if(project == "Test")
            _addOneFile(testTemplateFileName, sourceFileName, mapping);
        else
            _addOneFile(cppTemplateFileName, sourceFileName, mapping);
    }


    //resize the file (and back) to update its modification date--hack, but I couldn't think of anything better
    QFile cmakeFile(cmakeListsFileName);
    int sz = cmakeFile.size();
    cmakeFile.resize(sz + 1);
    cmakeFile.resize(sz);
}

bool AddTool::_addOneFile(QString templateName, QString targetName,
                          const QList<QPair<QRegExp, QString> > &mapping)
{
    qDebug() << "Adding file:" << targetName << "from" << templateName;

    { //block to make sure file processing is done when we get out of it
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
    }

    //Now add it to mercurial
    QProcess process;
    process.setWorkingDirectory(QFileInfo(targetName).absolutePath());
    process.start(QString("hg add ") + QFileInfo(targetName).fileName());
    if(!process.waitForFinished(3000)) //3 seconds delay
    {
        toolError(QString("Couldn't hg add the file: ") + targetName);
        return false;
    }

    FormatTool::formatFile(targetName);

    return true;
}

#include "AddTool.moc"
