/*--
    OneParamWidget.h  

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

#ifndef CORNUCOPIA_ONEPARAMWIDGET_H_INCLUDED
#define CORNUCOPIA_ONEPARAMWIDGET_H_INCLUDED

#include "defs.h"
#include "Parameters.h"
#include <QWidget>

namespace Ui
{
    class OneParamWidgetUi;
}

class ParamWidget;

class OneParamWidget : public QWidget
{
    Q_OBJECT
public:
    OneParamWidget(ParamWidget *paramWidget, QWidget *parent, const Cornu::Parameters::Parameter &param);

    const Cornu::Parameters::Parameter &parameter() const { return _param; }

public slots:
    void setValue(double value);
    void parametersChanged();

signals:
    void valueChanged(double value);
    //for internal use
    void spinBoxValueChanged(double value);
    void sliderValueChanged(int value);
    void infinitySet(bool inf);

private slots:
    void setSliderValue(int value);
    void setInfinity(bool inf);

private:
    double _getValue() const;
    double _fromSlider(int val);
    int _toSlider(double val);

    ParamWidget *_paramWidget;
    Cornu::Parameters::Parameter _param;
    bool _changing;
    double _value;
    bool _infinity;
    Ui::OneParamWidgetUi *_ui;

    static const int _sliderMarks;
};

class ParameterSetter : public QObject
{
    Q_OBJECT
public:
    ParameterSetter(ParamWidget *paramWidget, OneParamWidget *oneParamWidget);

public slots:
    void valueChanged(double);

signals:
    void setParameter(Cornu::Parameters::ParameterType parameter, double value);

private:
    Cornu::Parameters::ParameterType _parameter;
};

#endif //CORNUCOPIA_ONEPARAMWIDGET_H_INCLUDED
