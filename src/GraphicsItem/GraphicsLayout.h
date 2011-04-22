/* This file is part of Rocs,
     Copyright (C) 2011 by:
     Andreas Cord-Landwehr <cola@uni-paderborn.de>

     Rocs is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License, or
     (at your option) any later version.

     Rocs is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with Step; if not, write to the Free Software
     Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef GRAPHICSLAYOUT_H
#define GRAPHICSLAYOUT_H

#include <QObject>
#include "rocslib_export.h"

class ROCSLIB_EXPORT GraphicsLayout : public QObject {
    Q_OBJECT
public:
    static GraphicsLayout* self();
    
    ~GraphicsLayout();
    
    void setViewStyleDataNode(int style);
    void setViewStyleDataEdge(int style);
    
    int viewStyleDataNode() const;
    int viewStyleDataEdge() const;

private:
    GraphicsLayout( QObject* parent=0 );
    int _viewStyleDataNode;
    int _viewStyleDataEdge;

    static GraphicsLayout* _self;
    
signals:
    void changed();
};

#endif //GRAPHICSLAYOUT
