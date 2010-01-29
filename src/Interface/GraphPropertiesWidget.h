#ifndef SUI_GRAPHPROPERTIESWIDGET_H
#define SUI_GRAPHPROPERTIESWIDGET_H

/* This file is part of Rocs,
	 Copyright (C) 2008 by:
	 Tomaz Canabrava <tomaz.canabrava@gmail.com>
	 Ugo Sangiori <ugorox@gmail.com>

	 Rocs is free software; you can redistribute it and/or modify
	 it under the terms of the GNU General Public License as published by
	 the Free Software Foundation; either version 2 of the License, or
	 (at your option) any later version.

	 Rocs is distributed in the hope that it will be useful,
	 but WITHOUT ANY WARRANTY; without even the implied warranty of
	 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
	 GNU General Public License for more details.

	 You should have received a copy of the GNU General Public License
	 along with Step; if not, write to the Free Software
	 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA	02110-1301	USA
*/
#include <KButtonGroup>
#include "ui_GraphPropertiesWidget.h"

class Graph;
class Node;
class Edge;


/*! \brief Properties Area, not used yet. */
class MainWindow;
class GraphPropertiesWidget : public KButtonGroup, public Ui::GraphPropertiesWidget
{
    Q_OBJECT
public:
    GraphPropertiesWidget (Graph *g, MainWindow* parent = 0 );
    QRadioButton *radio() const;

private slots:
    void on__graphEdgeColorApplyNow_clicked();
    void on__graphEdgeColor_activated(QColor c);
    void on__graphName_textChanged(QString n);
    void on__graphNodeColorApplyNow_clicked();
    void on__graphNodeColor_activated(QColor c);
    void on__graphDelete_clicked();

    void on__graphOriented_toggled(bool b);
    void on__graphAutomate_toggled(bool b);
    void on__activateGraph_toggled(bool b);
    void on__showNodeNames_toggled(bool b);
    void on__showEdgeNames_toggled(bool b);
    void on__showNodeValues_toggled(bool b);
    void on__showEdgeValues_toggled(bool b);
    void on__graphHide_toggled(bool b);
    
  signals:
    void updateNeeded();

  private:
    Graph *_graph;
    MainWindow *_mainWindow;

};
#endif
