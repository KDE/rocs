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

class Edge : public QObject {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor);
    Q_PROPERTY(QString value READ value WRITE setValue);
    Q_PROPERTY(QString name READ name WRITE setName);
    
public:
    Edge(QObject *parent, Node *from, Node *to);
    ~Edge();
    int relativeIndex() const;
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
