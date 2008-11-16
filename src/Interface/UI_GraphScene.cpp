
/* This file is part of Rocs,
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>

   Rocs is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Rocs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Step; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "UI_GraphScene.h"
#include "UI_MainWindow.h"
#include "action_AbstractAction.h"

#include <QGraphicsSceneMouseEvent>
#include <KDebug>

GraphScene::GraphScene ( QObject* parent ) : QGraphicsScene ( parent ){

}

void GraphScene::setSceneRect(qreal width, qreal height)
{
  QGraphicsScene::setSceneRect(0,0,width,height);

  //Bottom Horizontal Line
  QGraphicsLineItem *lineItem = new QGraphicsLineItem(width, 0, width, height);
  lineItem->setEnabled(false);
  addItem(lineItem);

  // Right Vertical Line
  lineItem = new QGraphicsLineItem(0, height, width, height); 
  lineItem->setEnabled(false);
  addItem(lineItem);

  //Top Horizontal Line
  lineItem = new QGraphicsLineItem(0, 0, width, 0); 
  lineItem->setEnabled(false);
  addItem(lineItem);
  
  // Left Vertical Line
  
  lineItem = new QGraphicsLineItem(0, 0, 0, height); 
  lineItem->setEnabled(false);
  addItem(lineItem);
  
  kDebug() << "Setted the Scene Rect ";
  
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
  _action->executePress(mouseEvent->scenePos());
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
  _action->executeMove(mouseEvent->scenePos());
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
  _action->executeRelease(mouseEvent->scenePos());
}

void GraphScene::setAction(AbstractAction *action){
  _action = action;
}
