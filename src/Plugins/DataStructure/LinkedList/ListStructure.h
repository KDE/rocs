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

#ifndef LISTSTRUCTURE_H
#define LISTSTRUCTURE_H

#include "DataStructure.h"

class ListNode;
namespace Rocs{
class ListStructure : public DataStructure {
  Q_OBJECT
  public:

    ListStructure ( Document* parent = 0 );

    ListStructure(DataStructure& other);

    virtual ~ListStructure();
  public slots:

//     virtual void setEngine ( QScriptEngine* engine );

    virtual ListNode* addNode ( QString name );

    virtual Pointer* addEdge ( Data* from, Data* to );

    void arrangeNodes();

    QScriptValue front();
    QScriptValue createNode(const QString &name);


private:

    ListNode * _front;
    QParallelAnimationGroup* _animationGroup;
};
}
#endif // LISTSTRUCTURE_H
