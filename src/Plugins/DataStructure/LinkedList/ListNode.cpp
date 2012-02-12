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

#include "ListNode.h"
#include <Pointer.h>
#include "KDebug"

DataPtr ListNode::create(DataStructurePtr parent, int uniqueIdentifier, int dataType)
{
    return Data::create<ListNode>(parent, uniqueIdentifier, dataType);
}


ListNode::ListNode(DataStructurePtr parent, int uniqueIdentifier, int dataType):
    Data(parent, uniqueIdentifier, dataType)
{

}


ListNode::~ListNode()
{

}

QScriptValue ListNode::front(){
  if (boost::shared_ptr<ListNode> n = next()){
      return n->scriptValue();
  }
  return 0;
}

void ListNode::pointTo(ListNode* to )
{
    addPointer(to->getData());
}

boost::shared_ptr<ListNode> ListNode::next() const{
    if (out_pointers().count() == 1 ){
        if(boost::shared_ptr<ListNode> n = boost::static_pointer_cast<ListNode>( out_pointers().at(0)->to())){
          return n;
        }
    }
    return boost::shared_ptr<ListNode>();
}
