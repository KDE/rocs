/*
    This file is part of RootedTree (Rocs Plugin).
    Copyright 2012  Wagner Reck <wagner.reck@gmail.com>

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

#ifndef ROOTEDTREEPLUGIN_H
#define ROOTEDTREEPLUGIN_H

#include "DataStructureBackendInterface.h"
#include "Document.h"

namespace Rocs{

class /*ROCSLIB_EXPORT*/ RootedTreePlugin : public DataStructureBackendInterface {

public:
    RootedTreePlugin ( QObject* parent,  const QList<QVariant> & /* args*/ );

    virtual ~RootedTreePlugin();

    virtual DataStructurePtr convertToDataStructure ( DataStructurePtr ds, Document* parent);
    virtual DataStructurePtr createDataStructure ( Document* parent );

    bool canConvertFrom(Document*) const;

    virtual QGraphicsItem* pointerItem ( PointerPtr ) const;
    virtual QGraphicsItem* dataItem ( DataPtr ) const;

    virtual QLayout* dataExtraProperties ( DataPtr arg1, QWidget* arg2 ) const;

    virtual QLayout* pointerExtraProperties ( PointerPtr arg1, QWidget* arg2 ) const;

    virtual QLayout* dataStructureExtraProperties ( DataStructurePtr graph, QWidget* parentWidget ) const;

};
}
#endif // ROOTEDTREELUGIN_H
