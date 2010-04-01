/*--
    GroupSelWidget.h  

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

#ifndef GROUPSELWIDGET_H_INCLUDED
#define GROUPSELWIDGET_H_INCLUDED

#include "defs.h"

#include <QWidget>
#include <QSet>

class ScrollScene;
class QCheckBox;

class GroupSelWidget : public QWidget
{
    Q_OBJECT
public:
    GroupSelWidget(QWidget *parent);

    void setScene(ScrollScene *scene);

private slots:
    void sceneChanged();

private:
    ScrollScene *_scene;
    QSet<QString> _prevGroupSet;
};

class VisibilitySetter : public QObject
{
    Q_OBJECT
public:
    VisibilitySetter(QCheckBox *parent, ScrollScene *scene, QString group);

private slots:
    void visibilityChanged(int state);

private:
    ScrollScene *_scene;
    QString _group;
};

#endif //GROUPSELWIDGET_H_INCLUDED
