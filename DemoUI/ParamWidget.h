/*--
    ParamWidget.h  

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

#ifndef PARAMWIDGET_H_INCLUDED
#define PARAMWIDGET_H_INCLUDED

#include "defs.h"
#include "Parameters.h"
#include "Algorithm.h"

#include <QDockWidget>

namespace Ui
{
    class ParamWidgetUi;
}

class QListWidgetItem;

class ParamWidget : public QDockWidget
{
    Q_OBJECT
public:
    ParamWidget(QWidget *parent = NULL);
    ~ParamWidget();

    const Cornu::Parameters &parameters() const { return _parameters; }

public slots:
    void setParameter(Cornu::Parameters::ParameterType parameter, double value);
    void setAlgorithm(Cornu::AlgorithmStage stage, int algorithm);
    void setPresetFromList();
    void makePreset();

signals:
    void parametersChanged();

private:
    void _parametersChanged();
    void _addPreset(const Cornu::Parameters &params);

    Cornu::Parameters _parameters;
    Ui::ParamWidgetUi *_ui;

    std::vector<Cornu::Parameters> _presets;
};

#endif //PARAMWIDGET_H_INCLUDED
