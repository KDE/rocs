/*
    This file is part of Rocs.
    Copyright 2010-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2010       Wagner Reck <wagner.reck@gmail.com>
    Copyright 2012-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>

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

#include "rocs1fileformat.h"
#include "fileformats/fileformatinterface.h"
#include "graphdocument.h"
#include "node.h"
#include "edge.h"
#include <KAboutData>
#include <KLocalizedString>
#include <KPluginFactory>
#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QSaveFile>
#include <QDebug>

using namespace GraphTheory;

K_PLUGIN_FACTORY_WITH_JSON( FilePluginFactory,
                            "rocs1fileformat.json",
                            registerPlugin<Rocs1FileFormat>();)

class GraphTheory::Rocs1FileFormatPrivate
{
public:
    Rocs1FileFormatPrivate()
        : _buffer()
    {}

    QString _buffer;
};

Rocs1FileFormat::Rocs1FileFormat(QObject *parent, const QList< QVariant >&)
    : FileFormatInterface("rocs_rocs1fileformat", parent)
{
    d = new Rocs1FileFormatPrivate;
}

Rocs1FileFormat::~Rocs1FileFormat()
{
}

const QStringList Rocs1FileFormat::extensions() const
{
    return QStringList()
        << i18n("Rocs 1 Graph File Format (%1)", QString("*.graph"));
}

void Rocs1FileFormat::readFile()
{
    GraphDocumentPtr document = GraphDocument::create();

    QFile fileHandle(file().toLocalFile());
    document->setDocumentUrl(file());
    if (!fileHandle.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setError(CouldNotOpenFile, i18n("Could not open file \"%1\" in read mode: %2", file().toLocalFile(), fileHandle.errorString()));
        document->destroy();
        return;
    }
    QMap<int, NodePtr> nodeMap;
    QMap<int, NodeTypePtr> nodeTypeMap;
    QMap<int, EdgeTypePtr> edgeTypeMap;
    nodeTypeMap.insert(0, NodeType::create(document));
    edgeTypeMap.insert(0, EdgeType::create(document));

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
                    // DO NOTHING: data plugins not used anymore
                } else if (!dataLine.isEmpty()) {
                    break; // go to the last if and finish populating.
                }
                dataLine = in.readLine().simplified();
            }
        } else if (str.startsWith(QLatin1String("[DataStructure"))) {
            QString gName = str.section(' ', 1, 1);
            gName.remove(']');
            // DO NOTHING: data structures not used anymore
        }

        else if (str.startsWith(QLatin1String("[DataType"))) {
            QString identifier = str.section(' ', 1);
            identifier.remove(']');
            int tmpDataTypeId = identifier.toInt();
            if (!nodeTypeMap.contains(tmpDataTypeId)) {
                nodeTypeMap.insert(tmpDataTypeId, NodeType::create(document));
            }

            QString dataLine = in.readLine().simplified();
            while (!in.atEnd() && !dataLine.isEmpty()) {
                /**/ if (dataLine.startsWith(QLatin1String("Name :"))) {
                    nodeTypeMap[tmpDataTypeId]->setName(dataLine.section(' ', 2));
                } else if (dataLine.startsWith(QLatin1String("IconName :"))) {
                    QString iconString = dataLine.section(' ', 2);
                } else if (dataLine.startsWith(QLatin1String("Properties :"))) {
                    QStringList properties = dataLine.section(' ', 2).split(',');
                    foreach(const QString& property, properties) {
                        if (!property.isEmpty()) {
                            nodeTypeMap[tmpDataTypeId]->addDynamicProperty(property.section('=',0,0));
                        }
                    }
                } else if (!dataLine.isEmpty())               break;  // go to the last if and finish populating.
                dataLine = in.readLine().simplified();
            }
        }

        else if (str.startsWith(QLatin1String("[PointerType"))) {
            QString identifier = str.section(' ', 1);
            identifier.remove(']');
            int tmpEdgeTypeId = identifier.toInt();
            if (!edgeTypeMap.contains(tmpEdgeTypeId)) {
                edgeTypeMap.insert(tmpEdgeTypeId, EdgeType::create(document));
            }
            EdgeTypePtr tmpEdgeType = edgeTypeMap[tmpEdgeTypeId];

            QString dataLine = in.readLine().simplified();
            while (!in.atEnd() && !dataLine.isEmpty()) {
                /**/ if (dataLine.startsWith(QLatin1String("Name :"))) {
                    tmpEdgeType->setName(dataLine.section(' ', 2));
                } else if (dataLine.startsWith(QLatin1String("Direction :"))) {
                    if (dataLine.section(' ', 2) == "bidirectional") {
                        tmpEdgeType->setDirection(EdgeType::Bidirectional);
                    } else if (dataLine.section(' ', 2) == "unidirectional") {
                        tmpEdgeType->setDirection(EdgeType::Unidirectional);
                    }
                    else {
                        qCritical() << "Direction unset, use default direction of this data type backend.";
                    }
                } else if (dataLine.startsWith(QLatin1String("Properties :"))) {
                    QStringList properties = dataLine.section(' ', 2).split(',');
                    foreach(const QString& property, properties) {
                        if (!property.isEmpty()) {
                            tmpEdgeType->addDynamicProperty(property.section('=',0,0));
                        }
                    }
                } else if (!dataLine.isEmpty()) {
                    break;  // go to the last if and finish populating.
                }
                dataLine = in.readLine().simplified();
            }
        }

        else if (str.startsWith(QLatin1String("[Data "))) {
            NodePtr tmpNode;

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
            if (nodeTypeMap.contains(type)) {
                tmpNode = Node::create(document);
                tmpNode->setType(nodeTypeMap[type]);
            } else {
                qDebug() << "Create data element of type 0, since type " << type << " was not registered.";
                tmpNode = Node::create(document);
            }
            if (tmpNode) {
                tmpNode->setColor(color);
                tmpNode->setX(posX);
                tmpNode->setY(posY);

                // add to data element map
                QString identifier = str.section(' ', 1);
                identifier.remove(']');
                nodeMap.insert(identifier.toInt(), tmpNode);
            }
        }

        else if (str.startsWith(QLatin1String("[Pointer "))) {
            EdgePtr tmpEdge;
            QString eName = str.section(' ', 1, 1);
            eName.remove(']');

            QString nameFrom = eName.section("->", 0, 0);
            QString nameTo = eName.section("->", 1, 1);

            QString dataLine = in.readLine().simplified();
            QString value = "";
            int type = 0;
            QString color = "";
            while (!in.atEnd() && !dataLine.isEmpty()) {
                if (dataLine.startsWith(QLatin1String("value :")))     value = dataLine.section(' ', 2);
                else if (dataLine.startsWith(QLatin1String("type :")))      type = dataLine.section(' ', 2).toInt();
                else if (dataLine.startsWith(QLatin1String("color :")))     color = dataLine.section(' ', 2);
                else if (!dataLine.isEmpty())                break;  // go to the last if and finish populating.
                dataLine = in.readLine().simplified();
            }
            if (edgeTypeMap.contains(type)) {
                tmpEdge = Edge::create(nodeMap[nameFrom.toInt()], nodeMap[nameTo.toInt()]);
                tmpEdge->setType(edgeTypeMap[type]);
            } else {
                qDebug() << "Create pointer of type 0, since type " << type << " was not registered.";
                tmpEdge = Edge::create(nodeMap[nameFrom.toInt()], nodeMap[nameTo.toInt()]);
            }
        }
        // FIXME dynamic properties are not read
//         else if (str.contains(':')) {
//             QString propertyName = str.section(':', 0, 0).trimmed();
//             QString propertyValue = str.section(':', 1, 1).trimmed();
//             if (!propertyName.isEmpty()) {
//                 tmpObject->setProperty(propertyName.toUtf8() , propertyValue);
//             }
//         }
    }
    setGraphDocument(document);
    setError(None);
}

void Rocs1FileFormat::writeFile(GraphDocumentPtr graph)
{
    QSaveFile saveFile(!file().toLocalFile().endsWith(".graph") ? QString("%1.graph").arg(file().toLocalFile()) : file().toLocalFile());

    if (!saveFile.open(QIODevice::WriteOnly)) {
        setError(FileIsReadOnly, i18n("Could not open file \"%1\" in write mode: %2", file().fileName(), saveFile.errorString()));
        return;
    }

    QTextStream stream(&saveFile);
    stream.setCodec("UTF-8");
    // TODO test for successful serialization
    serialize(graph);
    stream << d->_buffer;

    if (!saveFile.commit()) {
        setError(FileIsReadOnly, i18n("Could not write data, aborting. Error: %1.", saveFile.errorString()));
        return;
    }

    setError(None);
}

QString Rocs1FileFormat::serialize(GraphDocumentPtr document)
{
    d->_buffer.clear();

    d->_buffer = QString("[Document Properties] \n")
              + QString("DataStructurePlugin : Graph") + QChar('\n')
              + QChar('\n');

    for(int typeID = 0; typeID < document->nodeTypes().length(); ++typeID) {
        QStringList properties;
        foreach (const QString &property, document->nodeTypes().at(typeID)->dynamicProperties()) {
            properties.append(property + QString('='));
        }

        d->_buffer += QString("[DataType %1]").arg(QString::number(typeID)) + QChar('\n')
            + QString("Name : ") + document->nodeTypes().at(typeID)->name() + QChar('\n')
            + QString("Properties : ") + properties.join(QChar(','))
            + QChar('\n');
        d->_buffer += QChar('\n');
    }

    for(int typeID = 0; typeID < document->edgeTypes().length(); ++typeID) {
        QStringList properties;
        foreach (const QString &property, document->edgeTypes().at(typeID)->dynamicProperties()) {
            properties.append(property + QString('='));
        }

        // set direction identifier
        QString direction = (document->edgeTypes().at(typeID)->direction() == EdgeType::Bidirectional)
            ? "bidirectional"
            : "unidirectional";

        d->_buffer += QString("[PointerType %1]").arg(QString::number(typeID)) + QChar('\n')
            + QString("Name : ") + document->edgeTypes().at(typeID)->name() + QChar('\n')
            + QString("Direction : ") + direction + QChar('\n')
            + QString("Properties : ") + properties.join(QChar(','))
            + QChar('\n');
        d->_buffer += QChar('\n');
    }

    d->_buffer += QString("[DataStructure 0] \n"); // all in one datastructure now
    foreach(NodePtr n, document->nodes()) {
        d->_buffer += QString("[Data %1]\n").arg(QString::number(n->id()));
        d->_buffer += QString("type : ") + QString::number(document->nodeTypes().indexOf(n->type())) + QChar('\n');
        foreach(const QString &property, n->dynamicProperties()) {
            d->_buffer += QString("%1 : %2 \n").arg(property).arg(n->dynamicProperty(property).toString());
        }
    }
    foreach(EdgePtr e, document->edges()) {
        d->_buffer += QString("[Pointer %1->%2]\n").
            arg(QString::number(e->from()->id())).
            arg(QString::number(e->to()->id())).toUtf8();
        d->_buffer += QString("type : ") + QString::number(document->edgeTypes().indexOf(e->type())) + QChar('\n');
        foreach(const QString &property, e->dynamicProperties()) {
            d->_buffer += QString("%1 : %2 \n").arg(property).arg(e->dynamicProperty(property).toString());
        }
    }

    qDebug() << "------- /// BEGIN internal file format /// -------";
    qDebug() << d->_buffer;
    qDebug() << "------- /// internal file format END /// -------";

    return d->_buffer;
}

#include "rocs1fileformat.moc"
