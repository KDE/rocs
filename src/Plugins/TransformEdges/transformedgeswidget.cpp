/*
    Rocs-Tools-Plugin: Generate graphs by specific patterns
    Copyright (C) 2011  Andreas Cord-Landwehr

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



#include <Document.h>
#include <DataStructure.h>
#include <DocumentManager.h>
#include "transformedgeswidget.h"
#include "ui_transformedgeswidget.h"

#include <cmath>
#include <KLocale>

#include <QtGui/QGridLayout>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtCore/QMap>
#include <QtCore/QPair>

#include <QDesktopWidget>


//TODO output usefull error message
namespace boost { void throw_exception( std::exception const & ) {} } // do noop on exception

class QPushButton;

TransformEdgesWidget::TransformEdgesWidget(Document* graphDoc, QWidget* parent)
    :   QWidget(parent)
{
    ui = new Ui::TransformEdgesWidget;
	ui->setupUi(this);
    graphDoc_ = graphDoc;
    
    // put widget at center of screen
    //TODO problems with two screens
    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen()); 
    QPoint center = rect.center();
    center.setX(center.x() - (this->width()/2));
    center.setY(center.y() - (this->height()/2));
    move(center);
}



TransformEdgesWidget::~TransformEdgesWidget()
{
    delete ui;
}



// void GenerateGraphWidget::generateStar(int numberSatelliteNodes)
// {
//     DocumentManager::self()->activeDocument()->activeDataStructure()->updateRelativeCenter();
//     QPointF center = DocumentManager::self()->activeDocument()->activeDataStructure()->relativeCenter();
//     
//     // compute radius such that nodes have space ~50 between each other
//     // circle that border-length of 2*PI*radius
//     int radius = 50*numberSatelliteNodes/(2*PI_);
// 
//     if ( !graphDoc_ ){
//       return;
//     }
// 
//     // use active data structure iff empty
//     DataStructure* graph = DocumentManager::self()->activeDocument()->activeDataStructure();
//     if (graph->dataList().size()>0)
//         graph = DocumentManager::self()->activeDocument()->addDataStructure( i18n("Star Graph") );
// 
//     // create mesh of NxN points
//     QList<Data*> starNodes;
// 
//     // create mesh nodes, store them in map
//     for (int i=1; i<=numberSatelliteNodes; i++) {
//         starNodes << graph->addData(
//             QString("%1").arg(i),
//             QPointF(sin(i*2*PI_/numberSatelliteNodes)*radius, cos(i*2*PI_/numberSatelliteNodes)*radius)+center
//         );
//     }
// 
//     // middle
//     starNodes.prepend( graph->addData(QString("center"),center) );
// 
//     // connect circle nodes
//     for (int i=1; i<=numberSatelliteNodes; i++) {
//         graph->addPointer (starNodes.at(0),starNodes.at(i));
//     }
// }
// 

#include "transformedgeswidget.moc"
