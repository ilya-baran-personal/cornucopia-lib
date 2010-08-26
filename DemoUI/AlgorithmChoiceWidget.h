/*--
    AlgorithmChoiceWidget.h  

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

#ifndef ALGORITHMCHOICEWIDGET_H_INCLUDED
#define ALGORITHMCHOICEWIDGET_H_INCLUDED

#include "defs.h"
#include "Algorithm.h"

#include <QGroupBox>
#include <QRadioButton>

class QVBoxLayout;
class ParamWidget;

class AlgorithmChoiceWidget : public QGroupBox
{
    Q_OBJECT
public:
    AlgorithmChoiceWidget(ParamWidget *paramWidget, QWidget *parent, Cornu::AlgorithmStage stage);

public slots:
    void parametersChanged();

private:
    ParamWidget *_paramWidget;
    Cornu::AlgorithmStage _stage;
    QVBoxLayout *_layout;
    std::vector<QRadioButton *> _buttons;
};

class AlgorithmSetter : public QObject
{
    Q_OBJECT
public:
    AlgorithmSetter(ParamWidget *paramWidget, QRadioButton *button, Cornu::AlgorithmStage stage, int algorithm);

public slots:
    void selected(bool);

signals:
    void setAlgorithm(Cornu::AlgorithmStage stage, int algorithm);

private:
    Cornu::AlgorithmStage _stage;
    int _algorithm;
};

#endif //ALGORITHMCHOICEWIDGET_H_INCLUDED
