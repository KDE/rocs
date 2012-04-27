/*
    This file is part of RootedTree (Rocs Plugin).
    Copyright 2012  Wagner Reck <wagner.reck@gmail.com>

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

#ifndef ROOTEDTREESTRUCTURE_H
#define ROOTEDTREESTRUCTURE_H

#include "DataStructure.h"
#include <boost/shared_ptr.hpp>

class RootedTreeNode;

class ROCSLIB_EXPORT RootedTreeStructure : public DataStructure {
    Q_OBJECT
    
    Q_PROPERTY(bool ShowAllPointers READ isShowingAllPointers WRITE setShowAllPointers NOTIFY showPointersChanged)
    
    RootedTreeNode* m_rootNode;
    
    bool m_showPointers;

    public:

    //to avoid hide some methods
    using DataStructure::addData;
    using DataStructure::addPointer;

    static DataStructurePtr create(Document *parent);
    static DataStructurePtr create(DataStructurePtr other, Document *parent);

    RootedTreeStructure ( Document* parent = 0 );
    ~RootedTreeStructure();
    void importStructure(DataStructurePtr other);

    PointerPtr addPointer(DataPtr from, DataPtr to, int dataType=0);
    DataPtr addData(QString name, int dataType=0);

    void setEngine(QScriptEngine* engine);
    DataPtr rootNode() const;
    
    
    /** return true if all the pointers need to be draw. */
    bool isShowingAllPointers() const;

    public slots:
        /** @brief Create a new data (tree node)
         Note that it only create but not insert it into the tree.
         */

        QScriptValue add_data(const QString& name);

        void set_root_node(RootedTreeNode* node);
        QScriptValue root_node() const;
	
	/** set if all the pointers should be draw or not. All pointers is a complete draw
	 * of this data structure, including pointers to parents and children division on nodes. */
	void setShowAllPointers(const bool v);

    signals:
	/** signal emitted on change of pointers visibility*/
        void showPointersChanged(const bool);

};
#endif // ROOTEDTREESTRUCTURE_H
