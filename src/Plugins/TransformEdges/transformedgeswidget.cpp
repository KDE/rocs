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
#include <limits.h>
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
#include <Data.h>


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
    if( ui->radioButtonMakeSpanningtree->isChecked() )
        makeSpanningTree( graph );
    if( ui->radioButtonConnectParts->isChecked() )
        connectComponents( graph );
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


qreal TransformEdgesWidget::makeSpanningTree(DataStructure* graph)
{
    GraphStructure* graphDS = qobject_cast<Rocs::GraphStructure *>(graph);
    if( !graphDS ) 
        return 0;

    QList<Data*> vertices = graphDS->dataList();
    int n = vertices.size();

    /*
     * the resulting spanning tree (MST)
     */
    QList< QPair<int,int> > MST;

    /*
     * distance[i] denotes the distance between node i and the minimum spanning
     * tree; initially this distance is infinity. Note that if i is already element
     * in MST distance[i] is only a temporary variable and its value is undefined.
     */
    QMap<int, qreal> distance;
    for( int i = 0; i < vertices.size(); i++) {
        distance[i] = std::numeric_limits<unsigned int>::max();
    }

    /*
     * Indicator variable that is true if node is in tree, false otherwise.
     * Initially all nodes are marked to be not in MST.
     */
    QMap<int, bool> inTree; 
    for (int i = 0; i < vertices.size(); i++) {
        inTree[i] = false;
    }

    /* weight[i][j] denotes distance between nodes i and j. If no
     * path is present between i and j in the previous tree the weight
     * must be set to 0.
     */
    QMap< QPair<int,int>, qreal> weight;

    for (int i=0; i<n; i++)
    {
    for (int j=0; j<n; j++)
    {
        if (i==j)   weight[QPair<int,int>(i,j)] = 0;
        
        PointerList out;
        if (graphDS->directed())  
            out = vertices[i]->out_pointers();
        else 
            out = vertices[i]->adjacent_pointers();
                
        for(int k=0; k<out.size(); k++) {
            if (out[k]->to()==vertices[j]) {
                if (out[k]->value()!="")
                    weight[QPair<int,int>(i,j)] = out[k]->value().toDouble();
                else
                    weight[QPair<int,int>(i,j)] = 1;
                
            }
        }
    }
    }

    /* 
     * successor[i] denotes the index of the node, to which i must be
     * linked to in order to get distance distance[i]
     */
    QMap< int, int> successor; 

    // start with first graph node
    inTree[0] = true;
    
    // update distances
    for (int i = 0; i < n; ++i) 
    {
        if ((weight[QPair<int,int>(0,i)] != 0) && (distance[i] > weight[QPair<int,int>(0,i)] )) {
            distance[i] = weight[QPair<int,int>(0,i)] ;
            successor[i] = 0;
        }
    }

    qreal total = 0;
    for(int treeSize = 1; treeSize < n; treeSize++) 
    {
        // Find node with the smallest distance to the tree
        int min = -1;
        for (int i = 0; i < n; ++i)
        {
            if (inTree[i]==false)
            {
                if ((min == -1) || (distance[min] > distance[i])) {
                    min = i;
                }
            }
        }

        // add node to tree
        MST << QPair<int,int>(successor[min], min);
        inTree[min] = 1;
        total += distance[min];

        // update distances
        for (int i = 0; i < n; ++i) 
        {
            if ((weight[QPair<int,int>(min,i)]  != 0) && (distance[i] > weight[QPair<int,int>(min,i)])) {
                distance[i] = weight[QPair<int,int>(min,i)];
                successor[i] = min;
            }
        }
    }
    
    // erase all graph edges
    removeAllEdges( graph );
    
    // refill with MST edges
    for (int i=0; i<MST.size(); i++) {
        Pointer* ptr = graph->addPointer(vertices[MST[i].first],vertices[MST[i].second]);
        
        if (weight[QPair<int,int>(MST[i].first,MST[i].second)]!=1) {
            QString s;
            s.setNum(weight[QPair<int,int>(MST[i].first,MST[i].second)]);
            ptr->setValue(s);
        }
    }

    return total;
}


void TransformEdgesWidget::connectComponents(DataStructure* graph)
{
    qDebug() << "now all remaining graph components should be connected";
    //TODO
}


#include "transformedgeswidget.moc"
