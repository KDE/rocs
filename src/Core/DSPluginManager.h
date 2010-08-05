/*

    Copyright (C) 2010  Wagner Reck (wagner.reck@gmail.com)

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

#ifndef DSPLUGINMANAGER_H
#define DSPLUGINMANAGER_H

#include <QObject>
#include "graph.h"
#include "rocslib_export.h"

class QLayout;
class QGraphicsItem;
class KPluginInfo;

namespace Rocs{

class DSPluginInterface;


class ROCSLIB_EXPORT DSPluginManager : public QObject {
  Q_OBJECT
    static DSPluginManager * self;
    DSPluginManager();
  public:
    /** Accessor to the manager's instance.*/
    static DSPluginManager * New();

    /** build a new graph using actual data structure. */
    static Graph * createNewDS(GraphDocument* parent, const QString& pluginName = QString());

    /** When the DS pluin  is changed, all the existent graphs must be 'rebuild' with the new DS. If is not possible to change to a new data structure without losing data, then user should be asked to continue or nor. If user say to not continue, all ready done job should be undo.*/
    static Graph * changeToDS(Graph*);

    /** return the list of avaliable data structure plugins.*/
    static const QStringList listOfDS();

    /** return informations about plugin.*/
    KPluginInfo pluginInfo ( DSPluginInterface* plugin ) const;


    QList < DSPluginInterface*> pluginsList();

    /** return the node graphic item that will be drawed by canvas . */
    QGraphicsItem * nodeItem(Node* node);
    /** return the edge graphic item that will be drawed by canvas . */
    QGraphicsItem * edgeItem(Edge* edge);


    /** Create extra widgets to node properties window. If no extra properties is needed the is returned 0*/
    QLayout * nodeExtraProperties(Node* node, QWidget * parent);
    /** Create extra widgets to edge properties window. If no extra properties is needed the is returned 0*/
    QLayout * edgeExtraProperties(Edge* edge, QWidget * parent);
    /** Create extra widgets to graph properties window. If no extra properties is needed the is returned 0*/
    QLayout * graphExtraProperties(Graph* graph, QWidget * parent);

  signals:
    /** signal emited when is changing the data structure to \newDS */
    void changingDS(const QString &newDS);

    /** this signal is emited at end of change of DS. This is used to say that the change was a success.*/
    void DSChangedTo(const QString &newDS);



  public slots:
      /** used to set change the data structure. */
    void changeActiveDS();
    void changeActiveDS( const QString& newDS);
    QString actualPlugin() const;

  private:

    class DSPluginManagerPrivate * const _d;
};
}
#endif // DSPLUGINMANAGER_H
