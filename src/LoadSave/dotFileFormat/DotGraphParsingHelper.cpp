/*
    This file is part of Rocs.
    Copyright 2006-2007  Gael de Chalendar <kleag@free.fr>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

    Rocs is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation, version 2.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA
*/


#include "DotGraphParsingHelper.h"
#include "DotGrammar.h"

#include <boost/throw_exception.hpp>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_distinct.hpp>
#include <boost/spirit/include/classic_loops.hpp>
#include <boost/spirit/include/classic_confix.hpp>


#include <iostream>

#include <kdebug.h>

#include <QFile>
#include<QUuid>
#include "DynamicPropertiesList.h"

using namespace std;

extern KGraphViewer::DotGraphParsingHelper* phelper;

namespace KGraphViewer
{
#define KGV_MAX_ITEMS_TO_LOAD std::numeric_limits<int>::max()

DotGraphParsingHelper::DotGraphParsingHelper():
    attrid(),
    valid(),
    attributed(),
    subdataTypeid(),
    unique(0),
    attributes(),
    dataStructureAttributes(),
    dataAttributes(),
    pointersAttributes(),
    dataStructureAttributesStack(),
    dataAttributesStack(),
    pointersAttributesStack(),
    edgebounds(),
    z(0),
    maxZ(0),
//   gs(0),
    gn(0),
    ge(0)
{
}

void DotGraphParsingHelper::setdataTypeelementattributes(QObject* ge, const AttributesMap& attributes)
{
    AttributesMap::const_iterator it, it_end;
    it = attributes.begin();
    it_end = attributes.end();
    for (; it != it_end; it++) {
        kDebug() << "    " << QString::fromStdString((*it).first) << "\t=\t'" << QString::fromStdString((*it).second) << "'";
        kDebug() << ge->metaObject()->className();
        if ((*it).first == "label" && strcmp(ge->metaObject()->className(), "Edge") == 0) {

            QString label = QString::fromUtf8((*it).second.c_str());
            label.replace("\\n", "\n");
            ge->setProperty("name", label);
        } else {
//       (*ge).attributes()[QString::fromStdString((*it).first)] =
            DynamicPropertiesList::New()->addProperty(ge, QString::fromStdString((*it).first));
            ge->setProperty((*it).first.c_str(), QString::fromStdString((*it).second));
        }
    }


}

void DotGraphParsingHelper::setdataStructureattributes()
{

    setdataTypeelementattributes(dataStructure.get(), dataStructureAttributes);
}

void DotGraphParsingHelper::setsubdataStructureattributes()
{
}

void DotGraphParsingHelper::setDataAttributes()
{

    if (gn == 0) {
        return;
    }
    setdataTypeelementattributes(gn, dataAttributes);
}

void DotGraphParsingHelper::setedgeattributes()
{
    setdataTypeelementattributes(ge, pointersAttributes);

}

void DotGraphParsingHelper::setattributedlist()
{
    if (attributed == "graph") {
        if (attributes.find("bb") != attributes.end()) {
            std::vector< int > v;
            parse_integers(attributes["bb"].c_str(), v);
            if (v.size() >= 4) {
                kDebug() << "setting width and height to " << v[2] << v[3];
            }
        }
        AttributesMap::const_iterator it, it_end;
        it = attributes.begin();
        it_end = attributes.end();
        for (; it != it_end; it++) {
            dataStructureAttributes[(*it).first] = (*it).second;
        }
    } else if (attributed == "datum") {
        AttributesMap::const_iterator it, it_end;
        it = attributes.begin();
        it_end = attributes.end();
        for (; it != it_end; it++) {
            dataAttributes[(*it).first] = (*it).second;
        }
    } else if (attributed == "edge") {
        AttributesMap::const_iterator it, it_end;
        it = attributes.begin();
        it_end = attributes.end();
        for (; it != it_end; it++) {
//       kDebug() << "    " << QString::fromStdString((*it).first) << " = " <<  QString::fromStdString((*it).second);
            pointersAttributes[(*it).first] = (*it).second;
        }
    }
    attributes.clear();
}

void DotGraphParsingHelper::createdatum(const std::string& datumid)
{
    QString id = QString::fromStdString(datumid);
    kDebug() << id;
    gn = dynamic_cast<Data*>(dataStructure->getData(id.toInt()).get()); //FIXME does not work
    if (gn == 0) { //&& dataType->data().size() < KGV_MAX_ITEMS_TO_LOAD)
        kDebug() << "Creating a new datum" << subdataTypeid;
        gn = dataStructure->addData(id).get();

        if (!subdataTypeid.isEmpty()) {
            gn->addDynamicProperty("SubGraph", subdataTypeid.last());
//       kDebug () << gn->dynamicPropertyNames();
        }

    }
    edgebounds.clear();
}

void DotGraphParsingHelper::createsubdataType()
{
}

void DotGraphParsingHelper::createedges()
{
    //FIXME the following logic is screwed, since the parser code tries to work with data names
    //      which was removed in favor for real IDs
//   kDebug();
    std::string datum1Name, datum2Name;
    datum1Name = edgebounds.front();
    edgebounds.pop_front();
    while (!edgebounds.empty()) {
        datum2Name = edgebounds.front();
        edgebounds.pop_front();

        DataPtr gn1 = dataStructure->getData(QString::fromStdString(datum1Name).toInt()); //FIXME does not work
        if (gn1 == 0) {
//       kDebug() << "new datum 1";
            gn1 = dataStructure->addData(QString::fromStdString(datum1Name));

            if (!subdataTypeid.isEmpty()) {
                gn1->addDynamicProperty("SubGraph", subdataTypeid.last());
            }
//             gn1 = dataType->datum(QString::fromStdString(datum1Name));
        }
        DataPtr gn2 = dataStructure->getData(QString::fromStdString(datum2Name).toInt()); //FIXME does not work
        if (gn2 == 0) {
//       kDebug() << "new datum 1";
            gn2 = dataStructure->addData(QString::fromStdString(datum2Name));

            if (!subdataTypeid.isEmpty()) {
                gn2->addDynamicProperty("SubGraph", subdataTypeid.last());
            }
        }
        if (gn1 == 0 || gn2 == 0) {
            kError() << "Unable to find or create edge bound(s) gn1=" << gn1 << "; gn2=" << gn2;
        }

        ge = dataStructure->addPointer(gn1, gn2).get();
        if (!subdataTypeid.isEmpty()) {
            ge->addDynamicProperty("SubGraph", subdataTypeid.last());
        }
        setedgeattributes();

        datum1Name = datum2Name;
    }
    edgebounds.clear();
}

void DotGraphParsingHelper::finalactions()
{

}

}
