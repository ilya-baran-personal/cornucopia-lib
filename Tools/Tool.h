/*--
    Tool.h  

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

    virtual QString name() const = 0;
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
