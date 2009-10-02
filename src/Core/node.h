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
#include <QVariant>
#include <QString>

#ifdef USING_QTSCRIPT
#include <QtScript>
#include "qtScriptBackend.h"
#endif

class Edge;

class Node : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX);
    Q_PROPERTY(qreal y READ y WRITE setY);
    Q_PROPERTY(qreal width READ width WRITE setWidth);
    Q_PROPERTY(QString name READ name WRITE setName);
    Q_PROPERTY(QString color READ color WRITE setColor);
    Q_PROPERTY(bool begin READ begin WRITE setBegin);
    Q_PROPERTY(bool end READ end WRITE setEnd);
    Q_PROPERTY(QString value READ value WRITE setValue);

public:
    Node(Graph *parent);
    ~Node();
    void addInEdge(Edge *e);
    void addOutEdge(Edge *e);
    void addSelfEdge(Edge *e);
    void removeEdge(Edge *e, int edgeList);
    void removeEdge(Edge *e, QList<Edge*> *list);
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
    void setEngine(	QtScriptBackend *_engine );
    QScriptValue createScriptArray(QList<Edge*> list);
#endif

public  slots:
    QList<Node*> adjacent_nodes() const;
    QList<Edge*> adjacent_edges() const;
    QList<Edge*> edges(Node *n);
    QList<Edge*> in_edges() const;
    QList<Edge*> out_edges() const;
    QList<Edge*> self_edges() const;

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
    void setValue(const QString& s);
    const QString& value() const;

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
    QList<Edge*> _in_edges;
    QList<Edge*> _out_edges;
    QList<Edge*> _self_edges;
    void empty(QList<Edge*> *list);

    //! fixed properties
    qreal _x;
    qreal _y;
    qreal _width;
    QString _name;
    QString _color;
    bool _begin;
    bool _end;
    QString _value;
    bool _changing;

    bool _showName;
    bool _showValue;
    Graph *_graph;

#ifdef USING_QTSCRIPT
    QScriptValue _scriptvalue;
    QtScriptBackend *_engine;
#endif

signals:
    void removed();
    void updateNeeded();
    void posChanged();
};

#endif
