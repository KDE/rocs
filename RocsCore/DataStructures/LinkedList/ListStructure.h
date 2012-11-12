/*
    This file is part of Rocs.
    Copyright 2011  Wagner Reck <wagner.reck@gmail.com>
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

#ifndef LISTSTRUCTURE_H
#define LISTSTRUCTURE_H

#include "rocs_liststructure_export.h"
#include <DataStructure.h>
#include <boost/shared_ptr.hpp>

class ListNode;
namespace Rocs
{
class ROCS_LISTSTRUCTURE_EXPORT ListStructure : public DataStructure
{
    Q_OBJECT
public:
    using DataStructure::remove;
    using DataStructure::addPointer;
    using DataStructure::addData;

    static DataStructurePtr create(Document *parent);
    static DataStructurePtr create(DataStructurePtr other, Document *parent);

    explicit ListStructure(Document* parent = 0);

    void importStructure(DataStructurePtr other);

    virtual ~ListStructure();

public slots:

    virtual DataPtr addData(const QString& name, int dataType = 0);

    virtual void remove(DataPtr n);

    virtual PointerPtr addPointer(DataPtr from, DataPtr to, int pointerType = 0);

    void arrangeNodes();

    virtual void remove(PointerPtr e);

    QScriptValue begin();
    void setBegin(Data* node);
    QScriptValue createNode(const QString& name);


private:
    void init();
    void createFront();

    boost::shared_ptr<ListNode> m_begin;
    QParallelAnimationGroup* m_animationGroup;

    bool m_building;
};
}
#endif // LISTSTRUCTURE_H
