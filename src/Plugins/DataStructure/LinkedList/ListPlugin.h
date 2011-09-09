/*
    This file is part of Rocs.
    Copyright 2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>

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

#ifndef LISTPLUGIN_H
#define LISTPLUGIN_H
#include "rocslib_export.h"
#include "DataStructurePluginInterface.h"

class Document;

namespace Rocs{

class /*ROCSLIB_EXPORT*/ ListPlugin : public DataStructurePluginInterface {
  public:
    ListPlugin ( QObject* parent,  const QList<QVariant> & /* args*/ );

    virtual ~ListPlugin();

    virtual DataStructurePtr convertToDataStructure ( DataStructurePtr graph, Document* parent );
    virtual DataStructurePtr createDataStructure ( Document* parent );

    virtual bool canConvertFrom(Document* doc) const;

    virtual QGraphicsItem* pointerItem ( PointerPtr ) const;
    virtual QGraphicsItem* dataItem ( DataPtr ) const;

    virtual QLayout* nodeExtraProperties ( DataPtr arg1, QWidget* arg2 ) const;



};
}
#endif // LISTPLUGIN_H
