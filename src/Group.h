/***************************************************************************
 *   Copyright (C) 2005-2006 by Tomaz Canabrava and Ugo Sangiori           *
 *   tomaz.canabrava@gmail.com                                             *
 *   ugorox@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#ifndef GROUP_H
#define GROUP_H

#include <QList>
#include <QRectF>
#include <QGraphicsItem>
#include "DiagramNode.h"
class DiagramScene;

class Group : public QObject, public QGraphicsRectItem
{
	Q_OBJECT
	private:
		DiagramScene *diagramScene;
		QRectF *box;
		QList<Node*> nodeList;	
	public:
		Group(DiagramScene *scene, QList<Node*> nodeList);
		~Group();
		void rePaint();
		void removeNode(Node *n);
	
	public slots:
	    QScriptValue nodes();
	};

#endif
