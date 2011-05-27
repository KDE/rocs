/* 
    This file is part of Rocs,
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of 
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SUI_GRAPHPROPERTIESWIDGET_H
#define SUI_GRAPHPROPERTIESWIDGET_H

#include <KButtonGroup>
#include "ui_GraphPropertiesWidget.h"

class DataStructure;
class Datum;
class Pointer;

/*! \brief Properties Area, not used yet. */
class MainWindow;
class GraphPropertiesWidget : public KButtonGroup, public Ui::GraphPropertiesWidget
{
    Q_OBJECT
public:
    GraphPropertiesWidget (DataStructure *g, MainWindow* parent = 0 );
    QRadioButton *radio() const;

private slots:
    void on__graphPointerColorApplyNow_clicked();
    void on__graphDatumColorApplyNow_clicked();
    void on__graphDelete_clicked();
    void on__graphName_textChanged(const QString& s);

    void on__activateGraph_toggled(bool b);
    void on__graphVisible_toggled(bool b);
    void setPointerDefaultColor(QColor c);
    void setDatumDefaultColor(QColor c);

  signals:
    void updateNeeded();
    void addGraph(QString name);
    void removeGraph();

  private:
    DataStructure *_graph;
    MainWindow *_mainWindow;

};
#endif
