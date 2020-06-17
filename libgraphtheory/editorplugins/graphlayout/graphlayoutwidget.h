/*
    Copyright (C) 2020  Dilson Almeida Guimarães <dilsonguim@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef GRAPHLAYOUTWIDGET_H
#define GRAPHLAYOUTWIDGET_H

#include "ui_graphlayoutwidget.h"
#include "typenames.h"
#include <QWidget>
#include <QDialog>

namespace GraphTheory {

class GraphLayoutWidget : public QDialog
{
    Q_OBJECT


public:
    explicit GraphLayoutWidget(GraphDocumentPtr document, QWidget *parent = 0);
    ~GraphLayoutWidget();

public slots:
    /**
     * Lay out the graph.
     */
    void layoutGraph();

    /**
    * Updates the seed used for generating pseudo-random numbers.
    */
    void setSeed(int seed);

    /**
    * Updates the area factor parameter of the layout algorithm.
    */
    void setAreaFactor(int areaFactor);

    /**
    * Updates the repelling force parameter of the layout algorithm.
    */
    void setRepellingForce(int repellingForce);

    /**
    * Updates the attraction force parameter of the layout algorithm.
    */
    void setAttractionForce(int attractionForce);

private:
    
    GraphDocumentPtr m_document;
    int m_seed;
    int m_areaFactor;
    int m_repellingForce;
    int m_attractionForce;

    Ui::GraphLayoutWidget *ui;
};
}

#endif
