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
#include <analitza/analyzer.h>
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
    importer.import(Data::staticMetaObject);
    importer.import(DataStructure::staticMetaObject);
    importer.import(Pointer::staticMetaObject);
    importer.import(AbstractRunBackend::staticMetaObject);
    
    analyzer.builtinMethods()->insertFunction(
        "backend",
        ExpressionType(ExpressionType::Lambda).addParameter(ExpressionType("AbstractRunBackend*")),
        new Func(qVariantFromValue<AbstractRunBackend*>(this))
    );
}

void AnalitzaBackend::addMetaClass(QMetaObject& o){
    importer.import(o);
}

void AnalitzaBackend::start()
{
    QTextStream stream(&_script);

    analyzer.importScript(&stream);
    if(!analyzer.isCorrect())
        emit sendDebug(analyzer.errors().join("\n"));
    emit finished();
}
