/*--
    Tool.h  

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

#ifndef TOOL_H_INCLUDED
#define TOOL_H_INCLUDED

#include <QWidget>
#include <QString>

class QTextStream;

class Tool : public QObject
{
    Q_OBJECT

public:
    Tool(QWidget *parent) : QObject(parent), _parentWidget(parent) {}

    virtual QString getName() const = 0;
    QWidget *parentWidget() const { return _parentWidget; }

public slots:
    virtual void execute() = 0;

protected:
    //utility functions
    void toolError(QString text);
    static void transformStream(QTextStream &from, QTextStream &to, const QList<QPair<QRegExp, QString> > &mapping);

private:
    QWidget *_parentWidget;
};

#endif //TOOL_H_INCLUDED
