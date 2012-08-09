/*
    This file is part of Rocs.
    Copyright 2009-2010  Tomaz Canabrava <tomaz.canabrava@gmail.com>

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

#ifndef GRAPHLAYERS_H
#define GRAPHLAYERS_H

#include <QScrollArea>
#include <QWidget>
#include <boost/shared_ptr.hpp>
#include "Rocs_Typedefs.h"
#include <QMap>

class DataStructure;
class QButtonGroup;
class KLineEdit;
class MainWindow;
class Document;
class DataStructurePropertiesWidget;

class GraphLayers : public QScrollArea
{
    Q_OBJECT
    QButtonGroup *_buttonGroup;
    MainWindow *_mainWindow;

protected:
    void resizeEvent(QResizeEvent * event);

public:
    GraphLayers(MainWindow *parent = 0);
    void setActiveDocument();

public slots:
    void btnADDClicked();
    void createLayer(DataStructurePtr g);
    void removeLayer(DataStructurePtr g);
    void selectFirstGraph();

signals:
    void createGraph(const QString& s);

private:
    Document *_activeDocument;
    QMap<DataStructurePtr, DataStructurePropertiesWidget*> m_layers;
};

#endif
