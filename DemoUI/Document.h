/*--
    Document.h  

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

#ifndef CORNUCOPIA_DOCUMENT_H_INCLUDED
#define CORNUCOPIA_DOCUMENT_H_INCLUDED

#include "defs.h"
#include "Parameters.h"
#include <vector>

#include <QObject>

class QDataStream;
class QTextStream;
CORNU_SMART_FORW_DECL(CurveSceneItem);

namespace Cornu
{
    CORNU_SMART_FORW_DECL(Polyline);
    CORNU_SMART_FORW_DECL(PrimitiveSequence);
}
class MainView;

class Document : public QObject
{
    Q_OBJECT
public:
    Document(MainView *view);

    void curveDrawn(Cornu::PolylineConstPtr polyline);

    void selectAt(const Eigen::Vector2d &point, bool shift, double radius);

public slots:
    void refitSelected();
    void deleteAll();
    void clearSelection();
    void open();
    void insert();
    void save();
    void selectAll();
    void deleteItem();

private:
    struct Sketch
    {
        Sketch() : selected(true), oversketch(-1) {}

        Cornu::PolylineConstPtr pts;
        QString name;
        Cornu::Parameters params;
        Cornu::PrimitiveSequenceConstPtr curve;
        CurveSceneItemPtr sceneItem;
        bool selected;
        int oversketch; //index of the sketch over which this one is sketched, or -1 if this is a new curve
    };

    void _selectionChanged() const;
    void _processSketch(int idx);

    bool _readFile(const QString &message, bool clear); //returns true on success
    Cornu::PolylineConstPtr _readPts(QDataStream &stream);
    void _writePts(QDataStream &stream, Cornu::PolylineConstPtr curve);
    std::vector<Sketch> _readNative(QTextStream &stream);
    void _writeNative(QTextStream &stream);
    QString _getNextSketchName();

    std::vector<Sketch> _sketches;
    MainView *_view;
    int _sketchIdx;
};

#endif //CORNUCOPIA_DOCUMENT_H_INCLUDED
