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

#ifndef EDGE_H
#define EDGE_H

#ifndef USING_QTSCRIPT
#define USING_QTSCRIPT 1
#endif

#include <QObject>
#include <QVariant>
#include <QtScript>
#include <QString>
#include <QColor>

#ifdef USING_QTSCRIPT
#include <QScriptValue>
#include "qtScriptBackend.h"
#endif

class Node;

/**
* \class Edge
* 
* This class is an edge on the graph. it holds the orientation, the from and the to nodes,auto
* and has a name, value and color properties.
* you can change all properties by 'edge.propertyName = new property' and access it's value
* by 'edge.propertyName' .
* new properties can be added on the fly via edge.addProp(propertyname) , and accessed by edge.propertyName.
*/

class Edge : public QObject {
    Q_OBJECT
    
    /*! all properties are accessible from the scripting engine via .propertyName */
    Q_PROPERTY(QColor color READ color WRITE setColor);
    Q_PROPERTY(QString value READ value WRITE setValue);
    Q_PROPERTY(QString name READ name WRITE setName);
    
public:
    /*! default constructor, takes a Graph* as a parent, and the nodes from and to that it will be bound to. */
    Edge(QObject *parent, Node *from, Node *to);
    
    /*! default destructor */
    ~Edge();
    
    /*! relative index is the index that this edge has relative to the nodes that it's bound to.
    eg. if the nodes have 2 or more edges connecteds between them, it will have a unique
    identifier relative to that nodes */
    int relativeIndex() const;
    
    /*! remove this node from the graph */
    void remove();
#ifdef USING_QTSCRIPT
    QScriptValue scriptValue() const;
    void setEngine(	QtScriptBackend *engine );
#endif

public  slots:
    Node* from() const;
    Node* to() const;
    const QString& value() const;
    void setValue (const QString& s);
    const QString& name() const;
    void setName (const QString& s);
    QColor color() const;
    void setColor(QColor s);
    
#ifdef USING_QTSCRIPT
    QScriptValue start();
    QScriptValue end();
    void self_remove();
#endif

private:
    Node *_from;
    Node *_to;
    int _relativeIndex;
    QString _value;
    QString _name;
    QColor _color;
    
#ifdef USING_QTSCRIPT
    QScriptValue _scriptvalue;
    QtScriptBackend *_engine;
#endif

};

#endif
