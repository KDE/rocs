/***************************************************************************
 * main.cpp
 * This file is part of the KDE project
 * copyright (C)2004-2007 by Tomaz Canabrava (tomaz.canabrava@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 ***************************************************************************/

#ifndef NODE_H
#define NODE_H

#ifndef USING_QTSCRIPT
#define USING_QTSCRIPT 1
#endif

#include <QObject>
#include <QList>
#include <QString>

#ifdef USING_QTSCRIPT
#include <QtScript>
#include "qtScriptBackend.h"
#endif
#include "rocslib_export.h"
#include "edge.h"
class Node;
typedef QList<Node*> NodeList;

class  ROCSLIB_EXPORT Node : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)
    Q_PROPERTY(qreal y READ y WRITE setY)
    Q_PROPERTY(qreal width READ width WRITE setWidth)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString color READ color WRITE setColor)
    Q_PROPERTY(bool begin READ begin WRITE setBegin)
    Q_PROPERTY(bool end READ end WRITE setEnd)
    Q_PROPERTY(QVariant value READ value WRITE setValue)
    Q_PROPERTY(QString iconPackage READ iconPackage WRITE setIconPackage)
    Q_PROPERTY(QString icon READ icon WRITE setIcon)
    
public:
    Node(Graph *parent);
    ~Node();
    void addInEdge(Edge *e);
    void addOutEdge(Edge *e);
    void addSelfEdge(Edge *e);
    void removeEdge(Edge *e, int edgeList);
    void removeEdge(Edge *e, EdgeList &list);
    void remove();
    enum EdgeLists {In, Out, Self};
    void startChange();
    void endChange();
    bool showName();
    bool showValue();
    void hideName(bool b);
    void hideValue(bool b);
    Graph *graph(){ return _graph; }

#ifdef USING_QTSCRIPT
    QScriptValue scriptValue() const;
    void setEngine(	QScriptEngine *_engine );
    QScriptValue createScriptArray(EdgeList list);
#endif
    


public  slots:
    NodeList adjacent_nodes() const;
    EdgeList adjacent_edges() const;
    EdgeList edges(Node *n);
    EdgeList in_edges() const;
    EdgeList out_edges() const;
    EdgeList self_edges() const;

    void setX(qreal x);
    void setY(qreal y);
    void setWidth(qreal w);
    void setPos(qreal x, qreal y);
    qreal x() const;
    qreal y() const;
    qreal width() const;
    void setColor(const QString& s);
    const QString& color() const;
    void setName(const QString& s);
    const QString& name() const;
    bool begin() const;
    bool end() const;
    void setBegin(bool begin = true);
    void setEnd(bool end = true);
    void setValue(const QVariant v);
    const QVariant value() const;
    void setIcon(const QString& s);
    const QString& icon() const;
    const QString& iconPackage() const;
    void setIconPackage(const QString& s);
    
    /** Add a property to this node
    * @param property Name of property
    * @param value Value of the property. arg2 shoud be different of QVariant::Invalid.
    */
    void addDinamicProperty(QString property, QVariant value);
    
    /** Remove property arg1 from this node. If property arg1 don't exist in this node, nothing is made.
    * @param property name os property to remove
    */
    void removeDinamicProperty(QString property);
    
#ifdef USING_QTSCRIPT

    QScriptValue adj_nodes();
    QScriptValue adj_edges();
    QScriptValue input_edges();
    QScriptValue output_edges();
    QScriptValue loop_edges();
    QScriptValue connected_edges(Node *n);
    void self_remove();
#endif

    Edge* addEdge(Node* to);
    
private:
    EdgeList _in_edges;
    EdgeList _out_edges;
    EdgeList _self_edges;
    void empty(EdgeList &list);

    //! fixed properties
    qreal _x;
    qreal _y;
    qreal _width;
    
    bool _begin;
    bool _end;
    bool _changing;
    bool _showName;
    bool _showValue;
    
    Graph *_graph;
    
    QString _name;
    QString _color;
    QString _iconpackage;
    QString _icon;
    
    QVariant _value;
    
#ifdef USING_QTSCRIPT
    QScriptValue _scriptvalue;
    QScriptEngine *_engine;
#endif

signals:
    void removed();
    void changed();
};


#endif
