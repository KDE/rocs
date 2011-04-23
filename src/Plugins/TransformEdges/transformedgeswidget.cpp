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
#include "../DataStructure/Graph/GraphStructure.h"

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
#include <Pointer.h>


//TODO output usefull error message
namespace boost { void throw_exception( std::exception const & ) {} } // do noop on exception
using namespace Rocs;

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


void TransformEdgesWidget::addDataStructures(QStringList dsNames)
{
    ui->dataStructuresCombo->insertItems(0, dsNames);
}


void TransformEdgesWidget::executeTransform()
{
    DataStructure* graph;
    QList<DataStructure*> dsList = DocumentManager::self()->activeDocument()->dataStructures();

    // no graph datastructures given at document
    if (ui->dataStructuresCombo->count()==0)
        return;

    graph = dsList[ui->dataStructuresCombo->currentIndex()];
    if (!graph)
        return;

    if( ui->radioButtonMakeComplete->isChecked() )
        makeComplete( graph );
    if( ui->radioButtonEraseEdges->isChecked() )
        removeAllEdges( graph );
    if( ui->radioButtonReverseEdges->isChecked() )
        reverseAllEdges( graph );
}

void TransformEdgesWidget::makeComplete( DataStructure* graph )
{
    if (graph)
    {
        foreach ( Pointer *e, graph->pointers() ) {
            graph->remove ( e );
        }
        
        int size_i = graph->dataList().size() - 1;
        for(int i = 0; i < size_i; ++i){
            for( int e = i+1; e < graph->dataList().size(); ++e){
                    graph->addPointer ( graph->dataList().at(i),graph->dataList().at(e) );
            }
        }
    }
}

void TransformEdgesWidget::removeAllEdges( DataStructure* graph )
{
    if (graph)
    {
        foreach ( Pointer *e, graph->pointers() ) {
            graph->remove ( e );
        }
    }
}

void TransformEdgesWidget::reverseAllEdges( DataStructure* graph )
{
    GraphStructure* graphDS = qobject_cast<Rocs::GraphStructure *>(graph);
    if( !graphDS ) 
        return;

    if (graphDS->directed()==false)
        return;
        
    QList< QPair<Data*, Data*> > newPointers;
    foreach ( Pointer *e, graphDS->pointers() ) {
        newPointers << QPair<Data*, Data*>(e->to(), e->from());
        graph->remove ( e );
    }
    
    for(int i=0; i<newPointers.count(); i++) {
        graph->addPointer(newPointers[i].first, newPointers[i].second);
    }
}

void TransformEdgesWidget::makeSpanningTree(DataStructure* graph)
{
    //TODO
}

#include "transformedgeswidget.moc"
