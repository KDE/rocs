/*
    This file is part of Rocs.
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012       Andreas Cord-Landwehr <cola@uni-paderborn.de>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "RocsGraphFileFormatPlugin.h"
#include "Document.h"
#include <KAboutData>
#include <KGenericFactory>
#include <KUrl>
#include <KSaveFile>
#include <QFile>
#include <DataStructure.h>
#include <Data.h>
#include <Pointer.h>
#include <DataType.h>
#include <PointerType.h>
#include <Modifiers/Topology.h>
#include <DataStructureBackendManager.h>
#include <DataStructureBackendInterface.h>

static const KAboutData pluginAboutData("rocs_rocsgraphfileformat",
                                        0,
                                        ki18nc("@title Displayed plugin name", "Rocs Graph File Backend"),
                                        "0.2",
                                        ki18n("Read and write Rocs Graph Files"),
                                        KAboutData::License_GPL_V2);

class RocsGraphFileFormatPluginPrivate
{
public:
    RocsGraphFileFormatPluginPrivate() :
        _buffer()
    {}

    QString _buffer;
};


RocsGraphFileFormatPlugin::RocsGraphFileFormatPlugin(QObject *parent)
    : GraphFilePluginInterface(&pluginAboutData, parent)
{
    d = new RocsGraphFileFormatPluginPrivate;
}


RocsGraphFileFormatPlugin::~RocsGraphFileFormatPlugin()
{
}


const QStringList RocsGraphFileFormatPlugin::extensions() const
{
    return QStringList()
           << i18n("*.graph|Rocs Graph File Format") + '\n';
}


void RocsGraphFileFormatPlugin::readFile()
{
    // This is a kind of a hack and should be solved better:
    // Document has no setter for the current plugin but on creation it gets created
    // with the plugin that is currently set as DataStructureBackendManager::actualPlugin.
    DataStructureBackendManager::self().activeBackend();
    Document* document = new Document("Untitled");

    QFile fileHandle(file().toLocalFile());
    document->setFileUrl(file());
    if (!fileHandle.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setError(CouldNotOpenFile, i18n("Could not open file \"%1\" in read mode: %2", file().toLocalFile(), fileHandle.errorString()));
        delete document;
        return;
    }
    DataStructurePtr tmpDataStructure;
    QObject *tmpObject = 0;
    QMap<int, DataPtr> dataMap;

    QTextStream in(&fileHandle);
    in.setCodec("UTF-8");

    while (!in.atEnd()) {
        QString str = in.readLine().simplified();

        if (str.startsWith('#')) { //! Ignore it, commented line.
            continue;
        } else if (str.startsWith(QLatin1String("[Document Properties]"))) {

            QString dataLine = in.readLine().simplified();
            while (!in.atEnd() && !dataLine.isEmpty()) {
                if (dataLine.startsWith(QLatin1String("DataStructurePlugin :"))) {
                    // set plugin by unique plugin identifier
                    QString pluginIdentifier = dataLine.section(' ', 2);
                    document->setBackend(pluginIdentifier);
                } else if (!dataLine.isEmpty()) {
                    break; // go to the last if and finish populating.
                }
                dataLine = in.readLine().simplified();
            }
            tmpObject = document;
        } else if (str.startsWith(QLatin1String("[DataStructure"))) {
            QString gName = str.section(' ', 1, 1);
            gName.remove(']');
            tmpDataStructure = document->addDataStructure(gName.toAscii());
            tmpObject = tmpDataStructure.get();
        }

        // plugin specific settings for data structure
        else if (str.startsWith(QLatin1String("X-plugin-")) && tmpObject && dynamic_cast<DataStructure*>(tmpObject)) {
            QString identifier = str.section(':', 0, 0).trimmed().remove("X-plugin-");
            QString property = str.section(':', 1, 1).trimmed();
            dynamic_cast<DataStructure*>(tmpObject)->getDataStructure()->setPluginProperty(identifier, property);
        }

        else if (str.startsWith(QLatin1String("[DataType"))) {
            QString identifier = str.section(' ', 1);
            identifier.remove(']');
            int tmpDataTypeId = identifier.toInt();
            if (tmpDataTypeId != 0) { // if == 0, this is default and default is automatically created
                tmpDataTypeId = document->registerDataType(QString(), identifier.toInt());
                if (tmpDataTypeId != identifier.toInt()) {
                    kWarning() << "Could not register already used data type << " << identifier.toInt()
                             << ": use identifier " << tmpDataTypeId << " instead.";
                }
            }
            DataTypePtr tmpDataType = document->dataType(tmpDataTypeId);

            QString dataLine = in.readLine().simplified();
            while (!in.atEnd() && !dataLine.isEmpty()) {
                /**/ if (dataLine.startsWith(QLatin1String("Name :")))      tmpDataType->setName(dataLine.section(' ', 2));
                else if (dataLine.startsWith(QLatin1String("IconName :"))) {
                    QString iconString = dataLine.section(' ', 2);
                    // remove rocs_ prefix
                    tmpDataType->setIcon(iconString.remove("rocs_"));
                } else if (dataLine.startsWith(QLatin1String("Properties :"))) {
                    QStringList properties = dataLine.section(' ', 2).split(',');
                    foreach(const QString& property, properties) {
                        if (!property.isEmpty()) {
                            tmpDataType->addProperty(property.section('=',0,0),property.section('=',1));
                        }
                    }
                } else if (dataLine.startsWith(QLatin1String("Color :")))     tmpDataType->setDefaultColor(QColor(dataLine.section(' ', 2)));
                else if (!dataLine.isEmpty())               break;  // go to the last if and finish populating.
                dataLine = in.readLine().simplified();
            }
        }

        else if (str.startsWith(QLatin1String("[PointerType"))) {
            QString identifier = str.section(' ', 1);
            identifier.remove(']');
            int tmpPointerTypeId = identifier.toInt();
            if (tmpPointerTypeId != 0) { // if == 0, this is default and default is automatically created
                tmpPointerTypeId = document->registerPointerType(QString(), identifier.toInt());
                if (tmpPointerTypeId != identifier.toInt()) {
                    kWarning() << "Could not register already used pointer type << " << identifier.toInt()
                             << ": use identifier " << tmpPointerTypeId << " instead.";
                }
            }
            PointerTypePtr tmpPointerType = document->pointerType(tmpPointerTypeId);

            QString dataLine = in.readLine().simplified();
            while (!in.atEnd() && !dataLine.isEmpty()) {
                /**/ if (dataLine.startsWith(QLatin1String("Name :"))) {
                    tmpPointerType->setName(dataLine.section(' ', 2));
                } else if (dataLine.startsWith(QLatin1String("Color :"))) {
                    tmpPointerType->setDefaultColor(QColor(dataLine.section(' ', 2)));
                } else if (dataLine.startsWith(QLatin1String("Direction :"))) {
                    if (dataLine.section(' ', 2) == "bidirectional") {
                        tmpPointerType->setDirection(PointerType::Bidirectional);
                    } else if (dataLine.section(' ', 2) == "unidirectional") {
                        tmpPointerType->setDirection(PointerType::Unidirectional);
                    }
                    else {
                        kWarning() << "Direction unset, use default direction of this data type backend.";
                    }
                } else if (dataLine.startsWith(QLatin1String("LineStyle :"))) {
                    tmpPointerType->setLineStyle(Qt::PenStyle(dataLine.section(' ', 2).toInt()));
                } else if (dataLine.startsWith(QLatin1String("Properties :"))) {
                    QStringList properties = dataLine.section(' ', 2).split(',');
                    foreach(const QString& property, properties) {
                        if (!property.isEmpty()) {
                            tmpPointerType->addProperty(property.section('=',0,0),property.section('=',1));
                        }
                    }
                } else if (!dataLine.isEmpty()) {
                    break;  // go to the last if and finish populating.
                }
                dataLine = in.readLine().simplified();
            }
        }

        else if (str.startsWith(QLatin1String("[Data "))) {
            DataPtr tmpData;

            QString dataLine = in.readLine().simplified();
            int type = 0;
            qreal posX = 0;
            qreal posY = 0;
            QString value = "";
            QString color = "";
            QString name = "";
            while (!in.atEnd() && !dataLine.isEmpty()) {
                /**/ if (dataLine.startsWith(QLatin1String("x :")))         posX = dataLine.section(' ', 2).toFloat();
                else if (dataLine.startsWith(QLatin1String("y :")))         posY = dataLine.section(' ', 2).toFloat();
                else if (dataLine.startsWith(QLatin1String("type :")))      type = dataLine.section(' ', 2).toInt();
                else if (dataLine.startsWith(QLatin1String("value :")))     value = dataLine.section(' ', 2);
                else if (dataLine.startsWith(QLatin1String("color :")))     color = dataLine.section(' ', 2);
                else if (dataLine.startsWith(QLatin1String("name :")))      name = dataLine.section(' ', 2);
                else if (!dataLine.isEmpty())               break;  // go to the last if and finish populating.
                dataLine = in.readLine().simplified();
            }
            if (document->dataTypeList().contains(type)) {
                tmpData = tmpDataStructure->addData(name, type);
            } else {
                kDebug() << "Create data element of type 0, since type " << type << " was not registered.";
                tmpData = tmpDataStructure->addData(name, 0);
            }
            if (tmpData) {
                tmpData->setColor(color);
                tmpData->setPos(posX, posY);

                // add to data element map
                QString identifier = str.section(' ', 1);
                identifier.remove(']');
                dataMap.insert(identifier.toInt(), tmpData);
                tmpObject = tmpData.get();
            }
        }

        else if (str.startsWith(QLatin1String("[Pointer "))) {
            PointerPtr tmpPointer;
            QString eName = str.section(' ', 1, 1);
            eName.remove(']');

            QString nameFrom = eName.section("->", 0, 0);
            QString nameTo = eName.section("->", 1, 1);

            QString dataLine = in.readLine().simplified();
            int width = 0;
            QString value = "";
            int type = 0;
            QString color = "";
            while (!in.atEnd() && !dataLine.isEmpty()) {
                /**/ if (dataLine.startsWith(QLatin1String("width :")))     width = dataLine.section(' ', 2).toInt();
                else if (dataLine.startsWith(QLatin1String("value :")))     value = dataLine.section(' ', 2);
                else if (dataLine.startsWith(QLatin1String("type :")))      type = dataLine.section(' ', 2).toInt();
                else if (dataLine.startsWith(QLatin1String("color :")))     color = dataLine.section(' ', 2);
                else if (!dataLine.isEmpty())                break;  // go to the last if and finish populating.
                dataLine = in.readLine().simplified();
            }
            if (document->pointerTypeList().contains(type)) {
                tmpPointer = tmpDataStructure->addPointer(dataMap[nameFrom.toInt()],dataMap[nameTo.toInt()], type);
            } else {
                kDebug() << "Create pointer of type 0, since type " << type << " was not registered.";
                tmpPointer = tmpDataStructure->addPointer(dataMap[nameFrom.toInt()],dataMap[nameTo.toInt()], 0);
            }
            if (tmpPointer) {
                tmpPointer->setWidth(width);
                tmpPointer->setColor(color);
                tmpObject = tmpPointer.get();
            }
        }

        else if (str.startsWith(QLatin1String("[Group"))) {
            /*QString gName = str.section(" ",1,1);
            gName.remove(']');
            tmpGroup = tmpDataStructure->addGroup(gName); */
        }

        else if (str.contains(':')) {
            QString propertyName = str.section(':', 0, 0).trimmed();
            QString propertyValue = str.section(':', 1, 1).trimmed();
            if (!propertyName.isEmpty()) {
                tmpObject->setProperty(propertyName.toUtf8() , propertyValue);
            }
        }
    }
    setGraphDocument(document);
    setError(None);
}

void RocsGraphFileFormatPlugin::writeFile(Document &graph)
{
    KSaveFile saveFile(!file().toLocalFile().endsWith(".graph") ? QString("%1.graph").arg(file().toLocalFile()) : file().toLocalFile());

    if (!saveFile.open()) {
        setError(FileIsReadOnly, i18n("Could not open file \"%1\" in write mode: %2", file().fileName(), saveFile.errorString()));
        return;
    }

    QTextStream stream(&saveFile);
    stream.setCodec("UTF-8");
    // TODO test for successful serialization
    serialize(graph);
    stream << d->_buffer;

    if (!saveFile.finalize()) {
        setError(FileIsReadOnly, i18n("Could not write data, aborting. Error: %1.", saveFile.errorString()));
        return;
    }

    setError(None);
}


QString RocsGraphFileFormatPlugin::serialize(const Document &document)
{
    d->_buffer.clear();

    d->_buffer = QString("[Document Properties] \n")
              % QString("DataStructurePlugin : ") % DataStructureBackendManager::self().activeBackend()->internalName() % QChar('\n')
              % QChar('\n');

    foreach(int dataTypeIdentifier, document.dataTypeList()) {
        QStringList properties;
        foreach (const QString& property, document.dataType(dataTypeIdentifier)->properties()) {
            properties.append(property + QString('=') + document.dataType(dataTypeIdentifier)->propertyDefaultValue(property).toString());
        }

        d->_buffer += QString("[DataType %1]").arg(QString::number(dataTypeIdentifier)) % QChar('\n')
            % QString("Name : ") % document.dataType(dataTypeIdentifier)->name() % QChar('\n')
            % QString("IconName : ") % document.dataType(dataTypeIdentifier)->iconName() % QChar('\n')
            % QString("Color : ") % document.dataType(dataTypeIdentifier)->defaultColor().name() % QChar('\n')
            % QString("Properties : ") % properties.join(QChar(','))
            % QChar('\n');
        d->_buffer += QChar('\n');
    }

    foreach(int pointerTypeIdentifier, document.pointerTypeList()) {
        QStringList properties;
        foreach (const QString& property, document.pointerType(pointerTypeIdentifier)->properties()) {
            properties.append(property + QString('=') + document.pointerType(pointerTypeIdentifier)->propertyDefaultValue(property).toString());
        }

        // set direction identifier
        QString direction = (document.pointerType(pointerTypeIdentifier)->direction() == PointerType::Bidirectional)
            ? "bidirectional"
            : "unidirectional";

        d->_buffer += QString("[PointerType %1]").arg(QString::number(pointerTypeIdentifier)) % QChar('\n')
            % QString("Name : ") % document.pointerType(pointerTypeIdentifier)->name() % QChar('\n')
            % QString("Color : ") % document.pointerType(pointerTypeIdentifier)->defaultColor().name() % QChar('\n')
            % QString("LineStyle : ") % QString::number(document.pointerType(pointerTypeIdentifier)->lineStyle()) % QChar('\n')
            % QString("Direction : ") % direction % QChar('\n')
            % QString("Properties : ") % properties.join(QChar(','))
            % QChar('\n');
        d->_buffer += QChar('\n');
    }

    // iterate over all data structures
    QList<DataStructurePtr>::const_iterator dataStructure = document.dataStructures().constBegin();
    int identifier=0;
    while (dataStructure != document.dataStructures().constEnd()) {
        d->_buffer += QString("[DataStructure %1] \n").arg(identifier++).toUtf8();
        QMap<QString, QString> properties = (*dataStructure)->pluginProperties();
        QMap<QString, QString>::const_iterator propertyIter = properties.constBegin();
        while (propertyIter != properties.constEnd()) {
            d->_buffer += QString("X-plugin-%1 : %2").arg(propertyIter.key()).arg(propertyIter.value()) % QChar('\n');
            ++propertyIter;
        }

        serializeProperties(dataStructure->get());

        foreach(int type, document.dataTypeList()) {
        foreach(DataPtr n, (*dataStructure)->dataList(type)) {
            d->_buffer += QString("[Data %1]\n").arg(QString::number(n->identifier()));
            d->_buffer += QString("type : ") % QString::number(n->dataType()) % QChar('\n');
            serializeProperties(n.get());
        }
        }

        foreach(int type, document.pointerTypeList()) {
        foreach(PointerPtr e, (*dataStructure)->pointers(type)) {
            d->_buffer += QString("[Pointer %1->%2]\n").
                arg(QString::number(e->from()->identifier())).
                arg(QString::number(e->to()->identifier())).toUtf8();
            d->_buffer += QString("type : ") % QString::number(e->pointerType()) % QChar('\n');
            serializeProperties(e.get());
        }
        }
        ++dataStructure;
    }
    kDebug() << "------- /// BEGIN internal file format /// -------";
    kDebug() << d->_buffer;
    kDebug() << "------- /// internal file format END /// -------";

    return d->_buffer;
}


void RocsGraphFileFormatPlugin::serializeProperties(QObject *o)
{
    const QMetaObject *metaObject = o->metaObject();
    int propertyCount = metaObject->propertyCount();

    for (int i = 0; i < propertyCount; ++i) {
        QMetaProperty metaProperty = metaObject->property(i);
        const char *name = metaProperty.name();
        QVariant value = o->property(name);

        if (QString("objectName").compare(metaProperty.name()) == 0) {
            continue;
        } else if (QString("name").compare(metaProperty.name()) == 0) {
            QString namevalue = QString("%1 : %2 \n").arg(name).arg(value.toString());
        }

        d->_buffer += QString("%1 : %2 \n").arg(name, value.toString());
    }

    QList<QByteArray> propertyNames = o->dynamicPropertyNames();
    foreach(const QByteArray & name, propertyNames) {
        QVariant value = o->property(name);
        d->_buffer +=  QString("%1 : %2 \n").arg(name, value.toString()).toUtf8();
    }

    d->_buffer += '\n';
}
