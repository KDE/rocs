/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  <copyright holder> <email>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef ABSTRACTRUNBACKEND_H
#define ABSTRACTRUNBACKEND_H

#include <QObject>
#include "rocslib_export.h"

class Document;


class ROCSLIB_EXPORT AbstractRunBackend : public QObject
{
    Q_OBJECT
public:
    explicit AbstractRunBackend(QObject* parent = 0);
    
   void setScript(const QString& s, Document *document);
   //virtual void setDocument(Document *document) = 0;
   void loadFile(const QString& file);
   
   void debug(const QString& s);
   void output(const QString& s);
   virtual bool isRunning(){ return false; }
   virtual void addMetaClass(const QMetaObject & o) = 0;
   virtual void addObject(const QString& name, QObject* o) = 0;
   
signals:
    void sendOutput(const QString& s);
    void sendDebug(const QString& s);
    void finished();
    
public slots:
    virtual void start() = 0;
    virtual void stop(){}
protected:
    QString _script;
    Document *_document;
    
};

#endif // ABSTRACTRUNBACKEND_H
