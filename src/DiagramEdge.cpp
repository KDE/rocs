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

#include <QPainter>
#include <math.h>
#include <QPainterPath>
#include <QPolygonF>
#include <QScriptValue>

#include "DiagramEdge.h"
#include "DiagramNode.h"
#include "DiagramScene.h"
#include "GraphEngine.h"

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

Edge::Edge(Node *sourceNode, Node *destNode, int arrowType, qreal length, bool show_length, DiagramScene *parent)
{
  this->arrowType = arrowType;
  this->length = length;
  this->show_length = show_length;
  this->arrowSize = 10;
  source = sourceNode;
  dest = destNode;
  source->addEdge(this);
  dest->addEdge(this);
  this->myLine = 0;
  diagramScene = parent;
  selected = false;
  adjust();
}

Edge::~Edge(){
	if (myLine) delete myLine;
	source->removeEdge(this);
	dest->removeEdge(this);
	source = 0;
	dest = 0;
	
	diagramScene -> removeEdge(this);
}

bool Edge::operator==(Edge *e)
{
	return (this == e) ? true : false;	
}

QScriptValue Edge::getFrom(){
  return getNode(0);
}

QScriptValue Edge::getTo(){
	return getNode(1);
}

QScriptValue Edge::getNode(int pos){
	if (diagramScene->engine == NULL) return QScriptValue();
	GraphEngine *e = diagramScene -> engine;
	
	//! search for the `To` node on the given list...
	int length = e->evaluate("nodes.length;").toInt32();
	for(int i = 0; i < length; i++)
	{
		QString n = "nodes["+QString::number(i)+"];";
		QScriptValue nodeFrom = e->evaluate(n);
		Node *node = qobject_cast<Node*>( nodeFrom.toQObject() );
		if (node == (pos == 0 ? source : dest) ) return nodeFrom;
	} 
	return QScriptValue();
}

void Edge::adjust()
{
    if (!source || !dest) return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();
    QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);

    prepareGeometryChange();
    sourcePoint = line.p1() + edgeOffset;
    destPoint = line.p2() - edgeOffset; 
}

QRectF Edge::boundingRect() const
{
  if (!source || !dest) return QRectF();

  qreal penWidth = 1;
  qreal extra = (penWidth + arrowSize) / 2.0;

  qreal x1 = sourcePoint.x();
  qreal y1 = sourcePoint.y();
  qreal x2 = destPoint.x();
  qreal y2 = destPoint.y();
  
  // Altura 
  if (y1 >= y2)
  {
    y1 += 8;
    y2 -= 8;
  }
  else{
    y1 -= 8;
    y2 += 8;
  }
  
  // Comprimento
  int len = QString::number(length).length();
  if (x1 >= x2)  x1 += 8*len;
  else           x2 += 8*len;  

  return QRectF(x1, y1, x2-x1, y2-y1).normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath Edge::shape() const
{
  if (myLine == NULL) return QPainterPath();
  
  QPointF p1, p2, p3, p4;
  
  //! descobrir em qual quadrante está cada coisa.
  if (myLine->p1().x() >= myLine->p2().x())
  {
    if (myLine->p1().y() >= myLine->p2().y())
    { //! Quarto quadrante
      p1.setX(myLine->p2().x() - 5); p1.setY(myLine->p2().y() + 5);
      p2.setX(myLine->p2().x() + 5); p2.setY(myLine->p2().y() - 5);
      p3.setX(myLine->p1().x() + 5); p3.setY(myLine->p1().y() - 5);
      p4.setX(myLine->p1().x() - 5); p4.setY(myLine->p1().y() + 5);  
    }
    else
    {                  //! Segundo quadrante
      p1.setX(myLine->p2().x() - 5); p1.setY(myLine->p2().y() - 5);
      p2.setX(myLine->p2().x() + 5); p2.setY(myLine->p2().y() + 5);
      p3.setX(myLine->p1().x() + 5); p3.setY(myLine->p1().y() + 5);
      p4.setX(myLine->p1().x() - 5); p4.setY(myLine->p1().y() - 5);  
    }
  }
  else
  {
    if (myLine->p1().y() >= myLine->p2().y())
    { //! Terceiro Quadrante
      p1.setX(myLine->p1().x() - 5); p1.setY(myLine->p1().y() - 5);
      p2.setX(myLine->p2().x() - 5); p2.setY(myLine->p2().y() - 5);
      p3.setX(myLine->p2().x() + 5); p3.setY(myLine->p2().y() + 5);
      p4.setX(myLine->p1().x() + 5); p4.setY(myLine->p1().y() + 5);    
    }
    else
    {                  // ! Primeiro Quadrante
      p1.setX(myLine->p1().x() + 5); p1.setY(myLine->p1().y() - 5);
      p2.setX(myLine->p2().x() + 5); p2.setY(myLine->p2().y() - 5);
      p3.setX(myLine->p2().x() - 5); p3.setY(myLine->p2().y() + 5);
      p4.setX(myLine->p1().x() - 5); p4.setY(myLine->p1().y() + 5);  
    }
  }

  //! desenha o quadradinho
  QPolygonF poly;
  poly << p1 << p2 << p3 << p4 << p1;
  QPainterPath p;
  p.addPolygon(poly);
  return p;
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
  if (!source || !dest)  return;

  if (myLine)
  {
    delete myLine;
    myLine = NULL;
  }
  
  myLine = new QLineF(sourcePoint, destPoint);
  painter->setPen(QPen( (selected ? Qt::red : Qt::black), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  painter->drawLine((*myLine));

  if (arrowType != NoArrow)
  {
    double angle = ::acos(myLine->dx() / myLine->length());
    if (myLine->dy() >= 0)
    {
      angle = TwoPi - angle;
    }

    painter->setBrush(Qt::black);
    QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize, cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,cos(angle - Pi + Pi / 3) * arrowSize);
    painter->drawPolygon(QPolygonF() << myLine->p2() << destArrowP1 << destArrowP2);

    if (arrowType == DoubleArrow)
    {
      QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize, cos(angle + Pi / 3) * arrowSize);
      QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize, cos(angle + Pi - Pi / 3) * arrowSize);
      painter->drawPolygon(QPolygonF() << myLine->p1() << sourceArrowP1 << sourceArrowP2);
    }
    painter->setBrush(Qt::NoBrush);
  }
  
  if (show_length == true)
  {
    qreal x = (myLine -> p1().x() + myLine -> p2().x()) / 2;
    qreal y = (myLine -> p1().y() + myLine -> p2().y()) / 2;  
    QString text(QString::number(length));
    painter->drawText(x,y, text);
  }
}

void Edge::setLength(double d)
{
  length = d;
  update();
}

void Edge::showLength(int i)
{
  show_length = (i == 0) ? false : true;
  update();
}
