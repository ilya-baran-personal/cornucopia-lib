/*--
    Document.h  

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

#ifndef DOCUMENT_H_INCLUDED
#define DOCUMENT_H_INCLUDED

#include "defs.h"
#include "Parameters.h"
#include <vector>

#include <QObject>

class QDataStream;
class QTextStream;
SMART_FORW_DECL(CurveSceneItem);

namespace Cornu
{
    SMART_FORW_DECL(Polyline);
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
        Sketch() : selected(true) {}

        Cornu::PolylineConstPtr pts;
        QString name;
        Cornu::Parameters params;
        CurveSceneItemPtr sceneItem;
        bool selected;
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

#endif //DOCUMENT_H_INCLUDED
