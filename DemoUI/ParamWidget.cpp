/*--
    ParamWidget.cpp  

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

#include "ParamWidget.h"
#include "Algorithm.h"
#include "ui_ParamWidget.h"
#include "OneParamWidget.h"
#include "AlgorithmChoiceWidget.h"

#include <QGroupBox>
#include <QRadioButton>
#include <QInputDialog>

using namespace std;
using namespace Eigen;

ParamWidget::ParamWidget(QWidget *parent)
    : QDockWidget(parent)
{
    _ui = new Ui::ParamWidgetUi();
    _ui->setupUi(this);

    //presets
    for(int i = 0; i < (int)Cornu::Parameters::presets().size(); ++i)
    {
        _addPreset(Cornu::Parameters::presets()[i]);
    }
    connect(_ui->presetList, SIGNAL(itemSelectionChanged()), this, SLOT(setPresetFromList()));

    //user parameters
    for(int i = 0; i < Cornu::Parameters::INTERNAL_PARAMETERS_MARKER; ++i)
    {
        _ui->paramLayout->insertWidget(_ui->paramLayout->count() - 1,
                                       new OneParamWidget(this, _ui->paramScrollContents, Cornu::Parameters::parameters()[i]));
    }

    //algorithms
    for(int i = 0; i < Cornu::NUM_ALGORITHM_STAGES; ++i)
    {
        Cornu::AlgorithmStage stage = (Cornu::AlgorithmStage)i;
        if(Cornu::AlgorithmBase::numAlgorithmsForStage(stage) < 2)
            continue;
    
        AlgorithmChoiceWidget *choiceWidget = new AlgorithmChoiceWidget(this, _ui->algScrollContents, stage);
        _ui->algLayout->insertWidget(_ui->algLayout->count() - 1, choiceWidget);
    }

    //internal parameters
    for(int i = Cornu::Parameters::INTERNAL_PARAMETERS_MARKER + 1; i < (int)Cornu::Parameters::parameters().size(); ++i)
    {
        _ui->internLayout->insertWidget(_ui->internLayout->count() - 1,
                                        new OneParamWidget(this, _ui->internScrollContents, Cornu::Parameters::parameters()[i]));
    }

    connect(_ui->presetButton, SIGNAL(clicked()), this, SLOT(makePreset()));
    connect(_ui->rerunButton, SIGNAL(clicked()), this, SIGNAL(rerunClicked()));

    _parametersChanged();
}

ParamWidget::~ParamWidget()
{
    delete _ui;
}

void ParamWidget::_addPreset(const Cornu::Parameters &params)
{
    _presets.push_back(params);
    QListWidgetItem *item = new QListWidgetItem(params.name().c_str(), _ui->presetList);
}

void ParamWidget::setParameter(Cornu::Parameters::ParameterType param, double value)
{
    if(_parameters.get(param) == value)
        return;
    _parameters.set(param, value);
    _parametersChanged();
}

void ParamWidget::setAlgorithm(Cornu::AlgorithmStage stage, int algorithm)
{
    if(_parameters.getAlgorithm(stage) == algorithm)
        return;
    _parameters.setAlgorithm(stage, algorithm);
    _parametersChanged();
}

void ParamWidget::setPresetFromList()
{
    for(int i = 0; i < (int)_presets.size(); ++i)
    {
        if(_ui->presetList->isItemSelected(_ui->presetList->item(i)))
        {
            if(_parameters == _presets[i])
                continue;
            _parameters = _presets[i];
            _parametersChanged();
            break;
        }
    }
}

void ParamWidget::makePreset()
{
    QString presetName = QInputDialog::getText(this, "Make Preset", "Enter the preset name");
    if(presetName.isNull())
        return;
    Cornu::Parameters newParams = _parameters;
    QByteArray name = presetName.toAscii();
    std::string nameString(name.constData(), name.length());
    newParams.setName(nameString);
    _addPreset(newParams);
    _parametersChanged();
}

void ParamWidget::_parametersChanged()
{
    bool somePresetSelected = false;
    for(int i = 0; i < (int)_presets.size(); ++i)
    {
        if(_presets[i] == _parameters)
        {
            _ui->presetList->setCurrentItem(_ui->presetList->item(i));
            somePresetSelected = true;
        }
        else
        {
            _ui->presetList->setItemSelected(_ui->presetList->item(i), false);
        }
    }

    _ui->presetButton->setEnabled(!somePresetSelected);

    emit parametersChanged();
}

#include "ParamWidget.moc"
