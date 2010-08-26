/*--
    GroupSelWidget.cpp  

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

#include "GroupSelWidget.h"
#include "ScrollScene.h"

#include <QCheckBox>
#include <QLayout>
#include <QPushButton>

using namespace std;
using namespace Eigen;

VisibilitySetter::VisibilitySetter(QCheckBox *parent, ScrollScene *scene, QString group)
: QObject(parent), _scene(scene), _group(group)
{
    connect(parent, SIGNAL(stateChanged(int)), this, SLOT(visibilityChanged(int)));
}

void VisibilitySetter::visibilityChanged(int state)
{
    _scene->setGroupVisible(_group, state != 0);
}

GroupSelWidget::GroupSelWidget(QWidget *parent)
    : QWidget(parent), _scene(NULL), _selectAll(NULL), _clearAll(NULL)
{
}

void GroupSelWidget::setScene(ScrollScene *scene)
{
    if(_selectAll == NULL)
    {
        _selectAll = new QPushButton("Show All", this);
        _clearAll = new QPushButton("Hide All", this);
        _selectAll->setFixedWidth(_selectAll->width());
        _clearAll->setFixedWidth(_selectAll->width());
        layout()->addWidget(_selectAll);
        layout()->addWidget(_clearAll);
        connect(_selectAll, SIGNAL(pressed()), this, SLOT(selectAll()));
        connect(_clearAll, SIGNAL(pressed()), this, SLOT(clearAll()));
    }

    if(_scene)
        disconnect(_scene, SIGNAL(sceneChanged()), this, SLOT(sceneChanged()));
    _scene = scene;
    connect(_scene, SIGNAL(sceneChanged()), this, SLOT(sceneChanged()));

    sceneChanged();
}

void GroupSelWidget::sceneChanged()
{
    QSet<QString> groupSet = _scene->getAllGroups();
    if(groupSet == _prevGroupSet)
        return;
    _prevGroupSet = groupSet;

    QList<QString> groups = groupSet.toList();
    qSort(groups);

    for(int i = 0; i < (int)_checkBoxes.size(); ++i)
        delete _checkBoxes[i];
    _checkBoxes.clear();

    int childWidth = 0;
    for(int i = 0; i < (int)groups.size(); ++i) {
        QCheckBox *box =  new QCheckBox(groups[i], this);
        QFont font = box->font();
        font.setPointSize(14);
        box->setFont(font);
        box->setCheckState(_scene->isGroupVisible(groups[i]) ? Qt::Checked : Qt::Unchecked);
        new VisibilitySetter(box, _scene, groups[i]);
        layout()->addWidget(box);
        childWidth = max(childWidth, box->sizeHint().width());
        _checkBoxes.push_back(box);
    }

    setMinimumWidth(150);

    QWidget *scrollArea = parentWidget()->parentWidget()->parentWidget();
    scrollArea->setMinimumWidth(childWidth + 20);
}

void GroupSelWidget::selectAll()
{
    for(int i = 0; i < (int)_checkBoxes.size(); ++i)
        _checkBoxes[i]->setCheckState(Qt::Checked);
}

void GroupSelWidget::clearAll()
{
    for(int i = 0; i < (int)_checkBoxes.size(); ++i)
        _checkBoxes[i]->setCheckState(Qt::Unchecked);
}

#include "GroupSelWidget.moc"
