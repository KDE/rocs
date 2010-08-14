/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

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

#include "ListNode.h"
#include "KDebug"

ListNode::ListNode(DataStructureBase* parent): Node(parent)
{

}


ListNode::~ListNode()
{

}

QScriptValue ListNode::front(){
  if (ListNode * n = next()){
      return n->scriptValue();
  }
  return 0;
}

void ListNode::pointTo(ListNode* to )
{
    addEdge(to);
}


void ListNode::setEngine(QScriptEngine* engine)
{
     _engine = engine;
    _scriptvalue = engine->newQObject(this);
}



ListNode * ListNode::next() const{
    if (out_edges().count() == 1 ){
        if(ListNode * n = qobject_cast<ListNode*>( out_edges().at(0)->to())){
          return n;
        }
    }
    return 0;
}