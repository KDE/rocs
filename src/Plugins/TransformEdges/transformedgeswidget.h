/*
    This file is part of Rocs.
    Copyright (C) 2011 Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#ifndef TRANSFORMEDGESWIDGET_H
#define TRANSFORMEDGESWIDGET_H

#include <QWidget>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>


class QGridLayout;
class Document;
class DataStructure;

namespace Ui {
    class TransformEdgesWidget;
}

class TransformEdgesWidget :
    public QWidget
{
    Q_OBJECT


    public:
        TransformEdgesWidget(Document* graphDoc, QWidget *parent=0);
        ~TransformEdgesWidget();

        /**
         * Add data structures to QComboBox of UI starting at position 0. Take care that
         * the data structure IDs must be given in increasing ID order without gaps.
         * Only data structures of documents of data structure type "Graph" are used.
         *
         * \param   QStringList dsNames names of data structures
         * \return  void
         */
        void addDataStructures(QStringList dsNames);

    public slots:
        void executeTransform();

    private:
        /**
         * Transform a given graph data structure to a complete graph.
         *
         * \param   DataStructure*  graph   the to be transformed graph
         */
        void makeComplete( DataStructure* graph );
        
        /**
         * Remove all edges from a given graph data structure.
         *
         * \param   DataStructure*  graph   the to be transformed graph
         * \return  void
         */        
        void removeAllEdges( DataStructure* graph );
        
        /**
         * Remove all edges of a given directed graph data structure. If an undirected
         * data structure is given no transformations are proceed.
         *
         * \param   DataStructure*  graph   the to be transformed graph
         * \return  void
         */
        void reverseAllEdges( DataStructure* graph );
        
        /**
         * Transform given graph to a spanning tree by executing Prim's minimum spanning tree (MST)
         * algorithm. All edges are assumed to have weight 1 if no weights are given. Otherwise,
         * with given weights the specified values are used.
         *
         * \param   DataStructure*  graph   the to be transformed graph
         * \return  qreal   total weight of MST
         */
        qreal makeSpanningTree( DataStructure* graph );


        Document* graphDoc_;
        Ui::TransformEdgesWidget *ui;
};

#endif // TRANSFORMEDGESWIDGET_H
