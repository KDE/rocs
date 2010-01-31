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
#include <QtScript>
#include <QString>

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

    /*! this property holds the color attribute. */
    Q_PROPERTY(QString color READ color WRITE setColor)

    /*! this property hold the value attribute */
    Q_PROPERTY(QString value READ value WRITE setValue)

    /*! this property holds the name of the edge. */
    Q_PROPERTY(QString name READ name WRITE setName)

    /*! this property holds the width of the edge */
    Q_PROPERTY(qreal width READ width WRITE setWidth)

    /*! this property holds the style of the edge */
    Q_PROPERTY(QString style READ style WRITE setStyle)

public:
    /*! default constructor, an edge connects two nodes.
    \p parent a Graph
    \p from the first node
    \p to the second node */
    Edge(Graph *parent, Node *from, Node *to);

    /*! default destructor */
    ~Edge();

    /*! relative index is the index that this edge has relative to the nodes that it's bound to.
    eg. if the nodes have 2 or more edges connecteds between them, it will have a unique
    identifier relative to that nodes

    \return the relativeIndex identifier.
    */
    int relativeIndex() const {
        return _relativeIndex;
    }

    /*! remove this node from the graph */
    void remove();

    bool showName();
    bool showValue();
    void hideName(bool b);
    void hideValue(bool b);
    

    Graph *graph(){return _graph; }
#ifdef USING_QTSCRIPT
    /*! if the qtscript is enabled for this rocs,
      this method returns the self-referenced script value for this edge.
      \return QScriptValue self reference for this node.
      */
    QScriptValue scriptValue() const;

    /*! if qtscript is enabled for this rocs,
    this method will set the engine for this single object
    \p engine the QScriptEngine that will work on the object */
    void setEngine(	QtScriptBackend *engine );
#endif

public  slots:
    /*! return the first node of this edge
      \return Node* pointer for the first node of this edge.
    */
    Node* from() const {
        return _from;
    }

    /*! return the second node of this edge
      \return Node* pointer for the second node of this edge.
    */
    Node* to() const {
        return _to;
    }

    /*! return the value of this edge
    \return the value of the edge.
    */
    const QString& value() const {
        return _value;
    }

    /*! sets the value attribute of this edge
    \p s the new value of this edge.
    */
    void setValue (const QString& s) {
        _value = s;
	emit changed();
    }

    /*! returns the name attribute of the edge.
      \return the name of the edge.
    */
    const QString& name() const {
        return _name;
    }

    /*! sets the name attribute of the edge
      \p s the new name of this edge
    */
    void setName (const QString& s) {
        _name = s;
	emit changed();
    }

    /*! gets the color attribute of the edge
      \return the string value of the edge.
    */
    const QString color() const {
        return _color;
    }

    /*! sets the color attribute of the edge
      \p s the new color of the edge in the format "#000000" or by it's english name ("red" for example)
    */
    void setColor(const QString& s) {
        _color = s;
	emit changed();
    }

    qreal width () const {
        return _width;
    }
    void setWidth(qreal w) {
        _width = w;
	emit changed();
    }

    const QString& style() const {
        return _style;
    }
    void setStyle(const QString& s) {
        _style = s;
	emit changed();
    }
    
    /** Add a property to this edge
    * @param property Name of property
    * @param value Value of the property. value shoud be different of QVariant::Invalid.
    */
    void addDinamicProperty(QString Property, QVariant value);
    
    /** Remove property arg1 from this edge. If property arg1 don't exist in this edge, nothing is made.
    * @param arg1 name os property to remove
    */
    void removeDinamicProperty(QString property);
    
#ifdef USING_QTSCRIPT
    /*! this method can be used inside of the script interface.
    \return the first node of this edge.
    */
    QScriptValue start();

    /*! this method can be used inside of the script interface,
    \return the last node of this edge
    */
    QScriptValue end();

    /*! this method can be used inside of the script interface,
     it will remove this edge from the graph.
     */
    void self_remove();

#endif

private:
    /*! the first node connected with this edge */
    Node *_from;

    /*! the second node connected with this edge */
    Node *_to;

    /*! the intex relative to the connected nodes if the graph is multi-edge-oriented. */
    int _relativeIndex;

    /*! the value of the node */
    QString _value;

    /*! the value of the name */
    QString _name;

    /*! the value of the color */
    QString _color;

    bool _showName;
    bool _showValue;

    QString _style;
    qreal _width;

     Graph *_graph;
#ifdef USING_QTSCRIPT
    /*! if the script interface is Qt-Script, this will hold the scriptValue self-reference. */
    QScriptValue _scriptvalue;

    /*! if the script interface is Qt-Script, this will hold  a pointer to the engine */
    QtScriptBackend *_engine;
#endif

signals:
    /*! emmited when this edge is removed. */
    void removed();
    /*! emmited when a node connected to this edge changes, or when this edge changes. */
    void changed();
};

typedef QList<Edge*> EdgeList;
#endif
