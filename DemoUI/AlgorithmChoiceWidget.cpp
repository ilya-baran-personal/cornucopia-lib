/*--
    AlgorithmChoiceWidget.cpp  

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

#include "AlgorithmChoiceWidget.h"
#include "ParamWidget.h"

#include <QVBoxLayout>

using namespace std;
using namespace Eigen;

AlgorithmChoiceWidget::AlgorithmChoiceWidget(ParamWidget *paramWidget, QWidget *parent, Cornu::AlgorithmStage stage)
    : QGroupBox(Cornu::AlgorithmBase::get(stage, 0)->stageName().c_str(), parent), _paramWidget(paramWidget), _stage(stage)
{
    int num = Cornu::AlgorithmBase::numAlgorithmsForStage(stage);
    _layout = new QVBoxLayout(this);

    for(int alg = 0; alg < num; ++alg)
    {
        QRadioButton *button = new QRadioButton(Cornu::AlgorithmBase::get(stage, alg)->name().c_str(), this);
        _layout->addWidget(button);
        _buttons.push_back(button);
        new AlgorithmSetter(paramWidget, button, _stage, alg);
    }

    connect(paramWidget, SIGNAL(parametersChanged()), this, SLOT(parametersChanged()));
}

void AlgorithmChoiceWidget::parametersChanged()
{
    int newAlgorithm = _paramWidget->parameters().getAlgorithm(_stage);
    if(_buttons[newAlgorithm]->isChecked())
        return;
    _buttons[newAlgorithm]->setChecked(true);
}

AlgorithmSetter::AlgorithmSetter(ParamWidget *paramWidget, QRadioButton *button, Cornu::AlgorithmStage stage, int algorithm)
    : QObject(button), _stage(stage), _algorithm(algorithm)
{
    connect(button, SIGNAL(toggled(bool)), this, SLOT(selected(bool)));
    connect(this, SIGNAL(setAlgorithm(Cornu::AlgorithmStage, int)), paramWidget, SLOT(setAlgorithm(Cornu::AlgorithmStage, int)));
}

void AlgorithmSetter::selected(bool state)
{
    if(state)
        emit setAlgorithm(_stage, _algorithm);
}

#include "AlgorithmChoiceWidget.moc"
