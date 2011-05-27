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

#ifndef ASSIGNVALUESWIDGET_H
#define ASSIGNVALUESWIDGET_H

#include <QWidget>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topology.hpp>


class QGridLayout;
class Document;
class DataStructure;

namespace Ui {
    class AssignValuesWidget;
}

class AssignValuesWidget :
    public QWidget
{
    Q_OBJECT

    enum AssignMethod {
        ID,
        UNIFORM_INTEGER,
        UNIFORM_FLOAT
    };

    public:
        AssignValuesWidget(Document* graphDoc, QWidget *parent=0);
        ~AssignValuesWidget();

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
        void assignValues();

    private:
        Document* graphDoc_;
        Ui::AssignValuesWidget *ui;
        
        /**
         * Assign integers in increasing order starting at 'start' to all nodes.
         *
         * \param   DataStructure*  ds  the to be changed data structure
         * \param   bool    overrideValues  if true, given values at variables are overwritten; otherwise not
         * \return  void
         */
        void assignIDsToNodes(DataStructure* ds, int start, bool overrideValues);
        
        /**
         * Assign integers in increasing order starting at 'start' to all edges.
         *
         * \param   DataStructure*  ds  the to be changed data structure
         * \param   bool    overrideValues  if true, given values at variables are overwritten; otherwise not
         * \return  void
         */
        void assignIDsToEdges(DataStructure* ds, int start, bool overrideValues);
        
        /**
         * Assign integers uniformly at random from range [lowerLimit,upperLimit] to nodes. Using Mersenne-Twister 
         * random number generator, initialized by 'seed'. If not lowerLimit < upperLimit the function returns without 
         * any operation.
         *
         * \param   DataStructure*  ds  the to be changed data structure
         * \param   qreal   lowerLimit  lower limit for random number interval
         * \param   qreal   upperLimit  upper limit for random number interval
         * \param   bool    overrideValues  if true, given values at variables are overwritten; otherwise not
         * \return  void
         */
        void assignIntegersToNodes(DataStructure* ds, int lowerLimit, int upperLimit, int seed, bool overrideValues);
        
        /**
         * Assign integers uniformly at random from range [lowerLimit,upperLimit] to edges. Using Mersenne-Twister 
         * random number generator, initialized by 'seed'. If not lowerLimit < upperLimit the function returns without 
         * any operation.
         *
         * \param   DataStructure*  ds  the to be changed data structure
         * \param   qreal   lowerLimit  lower limit for random number interval
         * \param   qreal   upperLimit  upper limit for random number interval
         * \param   bool    overrideValues  if true, given values at variables are overwritten; otherwise not
         * \return  void
         */
        void assignIntegersToEdges(DataStructure* ds, int lowerLimit, int upperLimit, int seed, bool overrideValues);
        
        /**
         * Assign float values uniformly at random from range [lowerLimit,upperLimit] to nodes. Using Mersenne-Twister 
         * random number generator, initialized by 'seed'. If not lowerLimit < upperLimit the function returns without 
         * any operation.
         *
         * \param   DataStructure*  ds  the to be changed data structure
         * \param   qreal   lowerLimit  lower limit for random number interval
         * \param   qreal   upperLimit  upper limit for random number interval
         * \param   bool    overrideValues  if true, given values at variables are overwritten; otherwise not
         * \return  void
         */
        void assignFloatsToNodes(DataStructure* ds, qreal lowerLimit, qreal upperLimit, int seed, bool overrideValues);
        
        /**
         * Assign float values uniformly at random from range [lowerLimit,upperLimit] to edges. Using Mersenne-Twister 
         * random number generator, initialized by 'seed'. If not lowerLimit < upperLimit the function returns without 
         * any operation.
         *
         * \param   DataStructure*  ds  the to be changed data structure
         * \param   qreal   lowerLimit  lower limit for random number interval
         * \param   qreal   upperLimit  upper limit for random number interval
         * \param   bool    overrideValues  if true, given values at variables are overwritten; otherwise not
         * \return  void
         */
        void assignFloatsToEdges(DataStructure* ds, qreal lowerLimit, qreal upperLimit, int seed, bool overrideValues);
};

#endif // ASSIGNVALUESWIDGET_H
