/*--
    ScrollScene.h  

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

#ifndef SCROLLSCENE_H_INCLUDED
#define SCROLLSCENE_H_INCLUDED

#include "defs.h"
#include "smart_ptr.h"

#include <vector>

#include <QObject>
#include <QRectF>
#include <QSet>

SMART_FORW_DECL(SceneItem);
class QPainter;
class QTransform;

class ScrollScene : public QObject
{
    Q_OBJECT
public:
    ScrollScene(QObject *parent = NULL) : QObject(parent) {}

    QRectF rect() const;
    void draw(QPainter *p, const QTransform &transform) const;

    void addItem(SceneItemPtr item);

    void clearGroups(QString groups);
    bool isGroupVisible(QString group) const;
    QSet<QString> getAllGroups() const;

    void setGroupVisible(QString group, bool visible);

signals:
    void sceneChanged();

protected:
    QSet<QString> _invisibleGroups;
    std::vector<SceneItemPtr> _items;
};

#endif //SCROLLSCENE_H_INCLUDED
