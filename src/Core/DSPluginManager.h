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

#include "rocslib_export.h"

class QLayout;
class QGraphicsItem;
class KPluginInfo;
class DataType;
class DataTypeDocument;
class Datum;
class Pointer;

namespace Rocs{

class DSPluginInterface;


class ROCSLIB_EXPORT DSPluginManager : public QObject {
  Q_OBJECT
    static DSPluginManager * self;
    DSPluginManager(QObject* parent);
    ~DSPluginManager();
  public:
    /** Accessor to the manager's instance.*/
    static DSPluginManager * instance(QObject * parent = 0);

    /** build a new dataType using actual data structure. */
    static DataType * createNewDS(DataTypeDocument* parent, const QString& pluginName = QString());

    /** When the DS pluin  is changed, all the existent dataTypes must be 'rebuild' with the new DS. If is not possible to change to a new data structure without losing data, then user should be asked to continue or nor. If user say to not continue, all ready done job should be undo.*/
    static DataType * changeToDS(DataType*);

    /** return the list of avaliable data structure plugins.*/
    static const QStringList listOfDS();

    /** return informations about plugin.*/
    KPluginInfo pluginInfo ( DSPluginInterface* plugin ) const;


    QList < DSPluginInterface*> pluginsList();

    /** return the datum dataTypeic item that will be drawed by canvas . */
    QGraphicsItem * datumItem(Datum* datum);
    /** return the pointer dataTypeic item that will be drawed by canvas . */
    QGraphicsItem * pointerItem(Pointer* pointer);


    /** Create extra widgets to datum properties window. If no extra properties is needed the is returned 0*/
    QLayout * datumExtraProperties(Datum* datum, QWidget * parent);
    /** Create extra widgets to pointer properties window. If no extra properties is needed the is returned 0*/
    QLayout * pointerExtraProperties(Pointer* pointer, QWidget * parent);
    /** Create extra widgets to dataType properties window. If no extra properties is needed the is returned 0*/
    QLayout * dataTypeExtraProperties(DataType* dataType, QWidget * parent);

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
