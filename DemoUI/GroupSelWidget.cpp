/*--
    GroupSelWidget.cpp  

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

#include "GroupSelWidget.h"
#include "ScrollScene.h"

#include <QCheckBox>
#include <QLayout>

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
: QWidget(parent), _scene(NULL)
{
}


void GroupSelWidget::setScene(ScrollScene *scene)
{
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

    QList<QWidget *> children = findChildren<QWidget *>();
    for(int i = 0; i < (int)children.size(); ++i)
        delete children[i];

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
    }

    setMinimumWidth(150);

    QWidget *scrollArea = parentWidget()->parentWidget()->parentWidget();
    scrollArea->setMinimumWidth(childWidth + 20);
}

#include "GroupSelWidget.moc"
