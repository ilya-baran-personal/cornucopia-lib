/*--
    FormatTool.h  

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

#ifndef FORMATTOOL_H_INCLUDED
#define FORMATTOOL_H_INCLUDED

#include "Tool.h"

class FormatTool : public Tool
{
    Q_OBJECT
public:
    FormatTool(QWidget *parent) : Tool(parent) {}

    //override
    QString name() const { return QString("Format files"); }

    static void formatFile(QString fileName);

public slots:
    //override
    void execute();

private:
    void _findAllCodeFiles(QString dir, QStringList &list);
};


#endif //FORMATTOOL_H_INCLUDED
