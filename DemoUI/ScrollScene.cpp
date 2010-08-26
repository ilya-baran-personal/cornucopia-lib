/*--
    ScrollScene.cpp  

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
    if(item->addToBeginning())
        _items.insert(_items.begin(), item);
    else
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

bool ScrollScene::isGroupVisible(QString group) const
{
    return !_invisibleGroups.contains(group);
}

void ScrollScene::setGroupVisible(QString group, bool visible)
{
    if(isGroupVisible(group) == visible)
        return;

    if(visible)
        _invisibleGroups.remove(group);
    else
        _invisibleGroups.insert(group);
    emit sceneChanged();
}

QSet<QString> ScrollScene::getAllGroups() const
{
    QSet<QString> out;

    for(int i = 0; i < (int)_items.size(); ++i)
        out.insert(_items[i]->group());

    return out;
}

#include "ScrollScene.moc"
