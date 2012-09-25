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


#include "analitzabackend.h"
#include <QtCore/QObject>
#include <analitza/analyzer.h>
#include <analitza/variables.h>
#include <analitza/importqobjectmetatype.h>

#include "Core/Data.h"
#include "Core/DataStructure.h"
#include "Core/Pointer.h"

using namespace Analitza;

Q_DECLARE_METATYPE(AbstractRunBackend*);

class Func : public Analitza::FunctionDefinition
{
public:
    Func(const QVariant& value) : m_value(value) {}
    
    virtual Expression operator()(const QList< Expression >& )
    {
        return Expression::constructCustomObject(m_value, 0);
    }
    
private:
    QVariant m_value;
};

AnalitzaBackend::AnalitzaBackend(QObject* parent)
: AbstractRunBackend(parent)
, analyzer()
, importer(&analyzer)
{
    addMetaClass(Data::staticMetaObject);
    addMetaClass(DataStructure::staticMetaObject);
    addMetaClass(Pointer::staticMetaObject);
    addMetaClass(Document::staticMetaObject);
    addMetaClass(AbstractRunBackend::staticMetaObject);
    
    addObject("backend", this);
    addMetaClass(_document->staticMetaObject);
}

void AnalitzaBackend::addMetaClass(const QMetaObject& o){
    if(!m_imported.contains(o.className())) {
        importer.import(o);
        m_imported.insert(o.className());
    }
}

void AnalitzaBackend::start()
{
    QTextStream stream(&_script);

    analyzer.importScript(&stream);
    if(!analyzer.isCorrect())
        emit sendDebug(analyzer.errors().join("\n"));
    emit finished();
    
    for(QMap<QString, ExpressionType>::const_iterator it=analyzer.builtinMethods()->varTypes().constBegin(); it!=analyzer.builtinMethods()->varTypes().constEnd(); ++it) {
        qDebug() << "...." << it.key() << ": "  << it.value().toString();
    }
    qDebug() << "lalala" << analyzer.type().toString();
}

void AnalitzaBackend::addObject(const QString& name, QObject* o)
{
    addMetaClass(*o->metaObject());
    analyzer.builtinMethods()->insertFunction(
        name,
        ExpressionType(ExpressionType::Lambda).addParameter(ExpressionType(o->metaObject()->className())),
        new Func(qVariantFromValue<QObject*>(o))
    );
//     analyzer.insertVariable(name, Expression::constructCustomObject(qVariantFromValue<QObject*>(o), 0));
}
