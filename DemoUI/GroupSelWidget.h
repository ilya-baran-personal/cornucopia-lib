/*--
    GroupSelWidget.h  

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

#ifndef CORNUCOPIA_GROUPSELWIDGET_H_INCLUDED
#define CORNUCOPIA_GROUPSELWIDGET_H_INCLUDED

#include "defs.h"

#include <QWidget>
#include <QSet>

class ScrollScene;
class QCheckBox;
class QPushButton;

class GroupSelWidget : public QWidget
{
    Q_OBJECT
public:
    GroupSelWidget(QWidget *parent);

    void setScene(ScrollScene *scene);

private slots:
    void sceneChanged();
    void selectAll();
    void clearAll();

private:
    ScrollScene *_scene;
    QSet<QString> _prevGroupSet;
    QList<QCheckBox *> _checkBoxes;
    QPushButton *_selectAll;
    QPushButton *_clearAll;
};

//Essentially binds a group to a signal-slot call
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

#endif //CORNUCOPIA_GROUPSELWIDGET_H_INCLUDED
