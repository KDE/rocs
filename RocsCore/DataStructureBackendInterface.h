/*
    This file is part of Rocs.
    Copyright 2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>

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


#ifndef DATASTRUCTUREBACKENDINTERFACE_H
#define DATASTRUCTUREBACKENDINTERFACE_H

//Qt classes
class QLayout;
class QGraphicsItem;

//Rocs Classes
class DataStructure;
class Data;
class Pointer;
class Document;

// KClasses
class KComponentData;

#include <QObject>
#include "RocsCoreExport.h"
#include "CoreTypes.h"


class ROCSLIB_EXPORT DataStructureBackendInterface : public QObject
{
    Q_OBJECT

public:
    DataStructureBackendInterface(const KComponentData& instance, QObject* parent);

    virtual ~DataStructureBackendInterface();
    virtual DataStructurePtr createDataStructure(Document * parent) = 0;
    virtual DataStructurePtr convertToDataStructure(DataStructurePtr, Document* parent) = 0;

    /** @brief Check if is possíble to convert from the doc document to this data structure.
     * If is possible to convert, is returned a true value, else, before return false, is good to implement in each scritp a message saying to user that not is possible to convert and asking if the conversion should proceed. If the user say to carry on, then true must be returned, otherwise false.
     */
    virtual bool canConvertFrom(Document* doc) const = 0;

    /** @brief return the translated name to be used in user visible strings
     */
    QString name();

    /** @brief return the internal name from plugin.
     * This string is not translated and should be used to differentiate
     * the data structures. It is useful when some piece of code is only
     * applicable to some kind of structures
     */
    QString internalName();

    virtual QGraphicsItem * dataItem(DataPtr data) const = 0;
    virtual QGraphicsItem * pointerItem(PointerPtr pointer) const = 0;

    virtual QLayout* dataExtraProperties(DataPtr data, QWidget* widget) const;
    virtual QLayout* pointerExtraProperties(PointerPtr pointer, QWidget* widget)const;
    virtual QLayout* dataStructureExtraProperties(DataStructurePtr dataStructure, QWidget* widget)const;

};


#endif // DataStructurePLUGININTERFACE_H
