/*
    SPDX-FileCopyrightText: 2020 Dilson Almeida Guimarães <dilsonguim@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
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
    explicit GraphLayoutWidget(GraphDocumentPtr document, QWidget *parent = nullptr);
    ~GraphLayoutWidget() override;

public Q_SLOTS:
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

    /**
    * Updates the index of the current tab.
    */
    void setCurrentTabIndex(const int index);


    /**
    * Updates the root parameter for the radial tree layout.
    */
    void setRoot(const int index);

    /**
    * Updates the node separation parameter for the radial tree layout.
    */
    void setNodeSeparation(const int nodeSeparation);

    /**
    * Updates the type of tree for the radial tree layout algorithm.
    */
    void freeTreeTypeToggle(const bool checked);
    void rootedTreeTypeToggle(const bool checked);
private:
    enum class TreeType {Free, Rooted}; 

    GraphDocumentPtr m_document;
    int m_seed;
    int m_areaFactor;
    int m_repellingForce;
    int m_attractionForce;
    int m_currentTabIndex;
    int m_root;
    TreeType m_treeType;
    int m_nodeSeparation;
    

    Ui::GraphLayoutWidget *ui;

    /**
    * Returns the name of the current tab.
    */
    QString getCurrentTabName() const;

    /**
    * Handles the application of the force-based layout.
    */
    void handleForceBasedLayout();

    /**
    * Handles the application of the radial tree layout.
    */
    void handleRadialTreeLayout();
};
}

#endif
