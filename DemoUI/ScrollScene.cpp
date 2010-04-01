/*--
    ScrollScene.cpp  

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

#include "ScrollScene.h"
#include "SceneItem.h"

using namespace std;
using namespace Eigen;

QRectF ScrollScene::rect() const
{
    QRectF out;
    for(int i = 0; i < (int)_items.size(); ++i)
    {
        if(_invisibleGroups.contains(_items[i]->group()))
            continue;
        out |= _items[i]->rect();
    }

    return out;
}

void ScrollScene::draw(QPainter *p, const QTransform &transform) const
{
    for(int i = 0; i < (int)_items.size(); ++i)
    {
        if(_invisibleGroups.contains(_items[i]->group()))
            continue;
        _items[i]->draw(p, transform);
    }
}

void ScrollScene::addItem(SceneItemPtr item)
{
    _items.push_back(item);
    emit sceneChanged();
}

void ScrollScene::clearGroups(QString groups)
{
    if(groups.isEmpty())
    {
        _items.clear();
        emit sceneChanged();
        return;
    }

    QRegExp groupExp(groups);

    int deleted = 0;
    for(int i = 0; i < (int)_items.size(); ++i)
    {
        if(groupExp.exactMatch(_items[i]->group()))
            ++deleted;
        else
            _items[i - deleted] = _items[i];
    }
    _items.resize((int)_items.size() - deleted);

    emit sceneChanged();
}

#include "ScrollScene.moc"
