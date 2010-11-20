/*--
    AddTool.h  

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

#ifndef CORNUCOPIA_ADDTOOL_H_INCLUDED
#define CORNUCOPIA_ADDTOOL_H_INCLUDED

#include "Tool.h"

class AddTool : public Tool
{
    Q_OBJECT
public:
    AddTool(QWidget *parent) : Tool(parent) {}

    //override
    QString name() const { return QString("Add..."); }

public slots:
    //override
    void execute();

private:
    void _add(QString fileName, bool header, bool source, bool moc, QString project);
    bool _addOneFile(QString templateName, QString targetName,
                     const QList<QPair<QRegExp, QString> >& mapping); //returns false on failure
};

#endif //CORNUCOPIA_ADDTOOL_H_INCLUDED
