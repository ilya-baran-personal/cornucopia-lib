/*--
    FormatTool.cpp  

    This file is part of the Cornucopia curve sketching library.
    Copyright (C) 2010 Ilya Baran (ibaran@mit.edu)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "FormatTool.h"
#include "config.h"

#include <QFileInfo>
#include <QDir>
#include <QDebug>

static const QString fileHeaderFileName = QString(CORNUCOPIA_SOURCE_DIR) + "/Tools/fileHeader.txt";

class _LineFormatter
{
public:
    _LineFormatter(QString fileName)
    {
        if(!fileName.isEmpty())
            mapping.push_back(qMakePair(QRegExp(QString("@FILENAME@")), fileName));
        mapping.push_back(qMakePair(QRegExp("\\t"), QString("    ")));
        mapping.push_back(qMakePair(QRegExp("\\n"), QString("")));
        mapping.push_back(qMakePair(QRegExp("\\r"), QString("")));
    }

    QByteArray formatLine(QByteArray &lineArray)
    {
        QString line = QString::fromAscii(lineArray.data(), lineArray.size());

        for(QList<QPair<QRegExp, QString> >::const_iterator it = mapping.constBegin();
            it != mapping.constEnd(); ++it)
        {
            line.replace(it->first, it->second);
        }

        QByteArray out = line.toAscii();
        out.append('\n');
        return out;
    }

private:
    QList<QPair<QRegExp, QString> > mapping;
};

void FormatTool::execute()
{
    qDebug() << "Formatting all files";

    //Find all the cpp and h files in the source tree
    QStringList files;
    _findAllCodeFiles(CORNUCOPIA_SOURCE_DIR, files);

    for(int i = 0; i < files.count(); ++i)
        _formatFile(files[i]);
}

void FormatTool::_findAllCodeFiles(QString dirName, QStringList &list)
{
    QDir dir(dirName);

    QStringList dirFilters;
    dirFilters << "[^.]*";
    dir.setNameFilters(dirFilters);

    QStringList subDirs = dir.entryList(QDir::Dirs);
    for(int i = 0; i < (int)subDirs.size(); ++i)
        _findAllCodeFiles(dirName + "/" + subDirs[i], list);

    QStringList fileFilters;
    fileFilters << "*.cpp" << "*.h" << "CMakeLists.txt";
    dir.setNameFilters(fileFilters);

    QStringList files = dir.entryList(QDir::Files);
    for(int i = 0; i < (int)files.size(); ++i)
        list.push_back(dirName + "/" + files[i]);
}

void FormatTool::_formatFile(QString fileName)
{
    qDebug() << "Processing file:" << fileName;

    QByteArray processed;

    //process header
    bool needsHeader = !fileName.endsWith(".txt"); //only .h and .cpp files need a header
    if(needsHeader)
    {
        _LineFormatter headerFormatter(QFileInfo(fileName).fileName());

        QFile header(fileHeaderFileName);
        if(!header.open(QIODevice::ReadOnly))
        {
            qDebug() << "Unable to read header file:" << fileHeaderFileName;
            return;
        }

        while(!header.atEnd())
        {
            QByteArray line = header.readLine();
            processed.append(headerFormatter.formatLine(line));
        }
    }

    //process file
    _LineFormatter fileFormatter("");

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to read file";
        return;
    }

    enum State
    {
        FirstLine,
        InHeader,
        AfterHeader
    } state = FirstLine;

    QByteArray oldFile;

    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        oldFile.append(line);
        switch(state)
        {
        case FirstLine:
            if(line.trimmed() == "/*--")
            {
                state = InHeader;
            }
            else
            {
                if(needsHeader)
                    processed.append('\n'); //add a break after the header
                processed.append(fileFormatter.formatLine(line));
                state = AfterHeader;
            }
            break;
        case InHeader:
            if(line.trimmed() == "*/")
            {
                state = AfterHeader;
            }
            break;
        case AfterHeader:
            processed.append(fileFormatter.formatLine(line));
            break;
        }
    }

    file.close();

    if(processed != oldFile)
    {
        if(!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "Unable to write file";
            return;
        }

        qDebug() << "Writing...";
        file.write(processed);
    }
}

#include "FormatTool.moc"
