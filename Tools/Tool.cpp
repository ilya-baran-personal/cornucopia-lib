/*--
    Tool.cpp  

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

#include "Tool.h"

#include <QMessageBox>
#include <QDebug>
#include <QTextStream>

void Tool::toolError(QString text)
{
    QMessageBox::critical(_parentWidget, "Tool error", text);
    qDebug() << "Error:" << text;
}

void Tool::transformStream(QTextStream &from, QTextStream &to, const QList<QPair<QRegExp, QString> > &mapping)
{
    while(!from.atEnd())
    {
        QString line = from.readLine();
        
        for(QList<QPair<QRegExp, QString> >::const_iterator it = mapping.constBegin();
            it != mapping.constEnd(); ++it)
        {
            line.replace(it->first, it->second);
        }

        to << line << "\n";
    }
}

#include "Tool.moc"
