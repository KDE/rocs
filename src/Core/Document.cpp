/*
    This file is part of Rocs.
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>
    Copyright 2010-2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2011-2012  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include "Document.h"
#include "DocumentManager.h"
#include "DataStructure.h"
#include <QString>
#include <KSaveFile>
#include <QByteArray>
#include <KDebug>
#include "Data.h"
#include "Pointer.h"
#include <QMap>

#include "PointerType.h"
#include "DataType.h"

#include <boost/shared_ptr.hpp>

//#include "dataStructureGroups.h"

#include "DataStructurePluginManager.h"
#include <GraphScene.h>
#include "DataStructurePluginInterface.h"
#include <KStandardDirs>
#include <DataItem.h>

class DocumentPrivate
{
public:
    DocumentPrivate() {}

    QString _buffer;
    QString _lastSavedDocumentPath;
    QString _name;
    qreal _left;
    qreal _right;
    qreal _top;
    qreal _bottom;
    qreal _minWidth;
    qreal _minHeight;
    bool _modified;
    bool _saved;
    DataStructurePtr _activeDataStructure;
    QPointer<DataStructurePluginInterface> _dataStructureType;
    QtScriptBackend* _engineBackend;
    QList< DataStructurePtr > _dataStructures;

    // data and pointer types
    QMap<int, DataTypePtr> _dataTypes;           // list of data types
    QMap<int, PointerTypePtr> _pointerTypes;        // list of pointer types

    QString _iconPackage;   // available icons for data types
};

Document::Document(const QString& name, qreal left, qreal right, qreal top, qreal bottom, QObject *parent)
    : QObject(parent), d(new DocumentPrivate())
{
    d->_name = name;
    d->_left = left;
    d->_right = right;
    d->_top = top;
    d->_bottom = bottom;
    d->_minWidth = 0;
    d->_minHeight = 0;
    d->_saved = false;
    d->_engineBackend = new QtScriptBackend(this);
    d->_dataStructureType = DataStructurePluginManager::self()->actualPlugin();
    d->_modified = false;

    d->_iconPackage = KGlobal::dirs()->locate("appdata", "iconpacks/default.svg");
    if (!DataItem::sharedRenderer(d->_iconPackage)) {
         DataItem::registerSharedRenderer(d->_iconPackage);
    }

    // default types
    d->_dataTypes.insert(0, DataType::create(this, 0));
    d->_pointerTypes.insert(0, PointerType::create(this, 0));

    qDebug() << "------=======------======";
    qDebug() << " Document Constructor ";
    qDebug() << d->_dataStructureType->name();
    qDebug() << "------=======------======";
}

void Document::setModified(const bool mod)
{
    d->_modified = mod;
}

Document::Document(const Document& gd)
    : QObject(gd.parent()), d(new DocumentPrivate())
{
//     QObject::setParent(gd.parent());
    d->_name = gd.name();
    d->_left = gd.left();
    d->_right = gd.right();
    d->_top = gd.top();
    d->_bottom = gd.bottom();
    d->_dataStructureType = DataStructurePluginManager::self()->actualPlugin();
    d->_engineBackend = new QtScriptBackend(this);

    d->_iconPackage = gd.iconPackage();
    if (!DataItem::sharedRenderer(d->_iconPackage)) {
         DataItem::registerSharedRenderer(d->_iconPackage);
    }

    // default types
    //FIXME add types from former document
    kDebug() << "Addeding just the default methods";
    d->_dataTypes.insert(0, DataType::create(this, 0));
    d->_pointerTypes.insert(0, PointerType::create(this, 0));

    for (int i = 0; i < gd.dataStructures().count(); ++i) {
        d->_dataStructures.append(
            DataStructurePluginManager::self()->changeToDataStructure(
                gd.d->_dataStructures.at(i), this
            )
        );
    }
}

// Default Destructor
Document::~Document()
{
    for (int i = 0; i < d->_dataStructures.size(); i ++) {
        d->_dataStructures.clear();
    }
}

int Document::registerDataType(QString name, int identifier)
{
    // create new identifier if identifier is already in use or 0
    if (identifier==0 || d->_dataTypes.contains(identifier)) {
        QList<int> usedIdentifier = d->_dataTypes.keys();
        qSort(usedIdentifier);
        identifier = usedIdentifier.last() + 1;
    }

    DataTypePtr dataType = DataType::create(this, identifier);
    dataType->setName(name);

    d->_dataTypes.insert(identifier, dataType);

    emit(dataTypeCreated(identifier));
    return identifier;
}

int Document::registerPointerType(QString name, int identifier)
{
    // create new identifier if identifier is already in use or 0
    if (identifier==0 || d->_pointerTypes.contains(identifier)) {
        QList<int> usedIdentifier = d->_pointerTypes.keys();
        qSort(usedIdentifier);
        identifier = usedIdentifier.last() + 1;
    }

    PointerTypePtr pointerType = PointerType::create(this, identifier);
    pointerType->setName(name);

    d->_pointerTypes.insert(identifier, pointerType);

    emit(pointerTypeCreated(identifier));
    return identifier;
}

QList< int > Document::dataTypeList() const
{
    return d->_dataTypes.keys();
}

QList< int > Document::pointerTypeList() const
{
    return d->_pointerTypes.keys();
}


const QString& Document::iconPackage() const
{
    return d->_iconPackage;
}


bool Document::removeDataType(int dataType)
{
    if (dataType == 0) {
        return false;
    }
    emit(dataTypeRemoved(dataType));
    return d->_dataTypes.remove(dataType) > 0;
}

bool Document::removePointerType(int pointerType)
{
    if (pointerType == 0 || !d->_pointerTypes.contains(pointerType)) {
        return false;
    }
    emit(pointerTypeRemoved(pointerType));
    return d->_pointerTypes.remove(pointerType) > 0;
}

DataTypePtr Document::dataType(int dataType) const
{
    Q_ASSERT(d->_dataTypes.contains(dataType));
    if (!d->_dataTypes.contains(dataType)) {
        return DataTypePtr();
    }
    return d->_dataTypes[dataType];
}

PointerTypePtr Document::pointerType(int pointerType) const
{
    Q_ASSERT(d->_pointerTypes.contains(pointerType));
    if (!d->_pointerTypes.contains(pointerType)) {
        return PointerTypePtr();
    }
    return d->_pointerTypes[pointerType];
}

QList< DataStructurePtr >& Document::dataStructures() const
{
    return d->_dataStructures;
}

QtScriptBackend * Document::engineBackend() const
{
    return d->_engineBackend;
}

// Sets the current file name of the DataStructure Collection
void Document::setName(const QString& name)
{
    d->_name = name;
    d->_modified = true;
    emit nameChanged(name);
}

// Gets the name of the DataStructure
QString Document::name() const
{
    return d->_name;
}

// gets the wheight of the drawable area
qreal Document::height() const
{
    return d->_bottom - d->_top;
}

// sets the width of the drawable area
qreal Document::width() const
{
    return d->_right - d->_left;
}

qreal Document::left() const
{
    return d->_left;
}

qreal Document::right() const
{
    return d->_right;
}

qreal Document::top() const
{
    return d->_top;
}

qreal Document::bottom() const
{
    return d->_bottom;
}

void Document::setLeft(qreal leftValue)
{
    d->_left = leftValue;
    //d->_modified = true;
}

void Document::setRight(qreal rightValue)
{
    d->_right = rightValue;
    //d->_modified = true;
}

void Document::setTop(qreal topValue)
{
    d->_top = topValue;
    //d->_modified = true;
}

void Document::setBottom(qreal bottomValue)
{
    d->_bottom = bottomValue;
    //d->_modified = true;
}

QRectF Document::size()
{
    return QRectF(d->_left, d->_top, d->_right - d->_left, d->_bottom - d->_top);
}

bool Document::isPointAtDocument(qreal x, qreal y)  const
{

    if (x < d->_left)      return false;
    if (x > d->_right)     return false;
    if (y < d->_top)       return false;
    if (y > d->_bottom)    return false;

    return true;
}

void Document::changeMinimalSize(qreal width, qreal height)
{
    if (width >= 0) d->_minWidth = width;
    if (height >= 0) d->_minHeight = height;

    if (width > d->_right - d->_left) {
        d->_left -= (d->_right - d->_left - width) / 2;
        d->_right += (d->_right - d->_left - width) / 2;
        emit resized();
    } else {
        resizeDocumentBorder(BorderLeft);
    }

    if (height > d->_bottom - d->_top) {
        d->_top -= (d->_bottom - d->_top - height) / 2;
        d->_bottom += (d->_bottom - d->_top - height) / 2;
        emit resized();
    } else {
        resizeDocumentBorder(BorderTop);
    }
}

void Document::resizeDocumentIncrease()
{
    int elements = dataStructures().size();
    for (int i = 0; i < elements; i++) {
        bool resizeDocument = false;
        foreach(DataPtr n,  dataStructures().at(i)->dataList()) {
            if (n->x() < d->_left + GraphScene::kBORDER) {
                setLeft(d->_left - GraphScene::kBORDER);
                resizeDocument = true;
            }
            if (n->x() > d->_right - GraphScene::kBORDER) {
                setRight(d->_right + GraphScene::kBORDER);
                resizeDocument = true;
            }
            if (n->y() < d->_top + GraphScene::kBORDER) {
                setTop(d->_top - GraphScene::kBORDER);
                resizeDocument = true;
            }
            if (n->y() > d->_bottom - GraphScene::kBORDER) {
                setBottom(d->_bottom + GraphScene::kBORDER);
                resizeDocument = true;
            }
        }
        if (resizeDocument) {
            emit resized();
        }
    }
}

void Document::resizeDocumentBorder(Document::Border orientation)
{
    bool empty = true;
    int elements = dataStructures().size();

    // scans doubled border of specified size: if empty or not
    for (int i = 0; i < elements; i++) {
        foreach(DataPtr n,  dataStructures().at(i)->dataList()) {
            switch (orientation) {
            case BorderLeft: {
                if (n != 0 && n->x() < d->_left + GraphScene::kBORDER * 2) empty = false;
                break;
            }
            case BorderRight: {
                if (n != 0 && n->x() > d->_right - GraphScene::kBORDER * 2) empty = false;
                break;
            }
            case BorderTop: {
                if (n != 0 && n->y() < d->_top + GraphScene::kBORDER * 2) empty = false;
                break;
            }
            case BorderBottom: {
                if (n != 0 && n->y() > d->_bottom - GraphScene::kBORDER * 2) empty = false;
                break;
            }
            }
        }
    }

    //TODO connect to graphvisualeditor-size
    if (empty == true) {
        switch (orientation) {
        case BorderLeft: {
            if (d->_right - d->_left < GraphScene::kBORDER * 4)
                return;
            setLeft(d->_left + GraphScene::kBORDER);
            emit resized();
            break;
        }
        case BorderRight: {
            if (d->_right - d->_left < GraphScene::kBORDER * 4)
                return;
            setRight(d->_right - GraphScene::kBORDER);
            emit resized();
            break;
        }
        case BorderTop: {
            if (d->_bottom - d->_top < GraphScene::kBORDER * 4)
                return;
            setTop(d->_top + GraphScene::kBORDER);
            emit resized();
            break;
        }
        case BorderBottom: {
            if (d->_bottom - d->_top < GraphScene::kBORDER * 4)
                return;
            setBottom(d->_bottom - GraphScene::kBORDER);
            emit resized();
            break;
        }
        }
    }
}

bool Document::isPointAtDocument(QPointF point)  const
{
    return isPointAtDocument(point.x(), point.y());
}

bool Document::isModified() const
{
    return d->_modified;
}

void Document::cleanUpBeforeConvert()
{
    foreach(DataStructurePtr ds, d->_dataStructures)
    ds->cleanUpBeforeConvert();
}


void Document::setActiveDataStructure(DataStructurePtr g)
{
    if (d->_dataStructures.indexOf(g) != -1) {
        d->_activeDataStructure = g;
        emit activeDataStructureChanged(g);
        d->_modified = true;
    }
}

DataStructurePtr Document::addDataStructure(QString name)
{
    DataStructurePtr g = DataStructurePluginManager::self()->createNewDataStructure(this,
                         d->_dataStructureType->name());
    if (name.isEmpty()) {
        // find unused name
        QList<QString> usedNames;
        foreach(DataStructurePtr dataStructure, d->_dataStructures) {
            usedNames.append(dataStructure->name());
        }
        // For at least one i in this range, the name is not used, yet.
        for (int i = 0; i < dataStructures().length() + 1; ++i) {
            name = QString("%1%2").arg(d->_dataStructureType->name()).arg(i);
            if (!usedNames.contains(name)) {
                break;
            }
        }
    }
    g->setName(name);
    d->_dataStructures.append(g);
    d->_activeDataStructure = g;
    d->_modified = true;
    connect(g.get(), SIGNAL(changed()), this, SLOT(setModified()));
    emit dataStructureCreated(g);
    return g;
}

void Document::save()
{
    Q_ASSERT(!fileUrl().isEmpty());
    saveAsInternalFormat(fileUrl());
}

void Document::saveAs(const QString& fileUrl)
{
    saveAsInternalFormat(fileUrl);
}

const QString& Document::fileUrl() const
{
    return d->_lastSavedDocumentPath;
}

void Document::remove(DataStructurePtr dataStructure)
{
    d->_dataStructures.removeOne(dataStructure);
    d->_modified = true;
}

/**
 * A Document Internal format may look like this:
 *
 * #                                '#'s are comments.
 *
 * [Document Properties]            # Canvas Size and Data Structure initialization.
 * Width  : [integer]
 * Height : [integer]
 * DataStructurePlugin : [string]
 *
 * # data types
 * [DataType Identifier1]
 * Name     : [string]
 * IconName : [string]
 * Color    : [RGB in hex]
 *
 * # pointer types
 * [PointerType Identifier1]
 * Name  : [string]
 * Color : [RGB in hex]
 *
 * # Data Structure Definitions.
 * # Data Structures are layered, so there can be N of them.
 * # Every Data and Pointer below a declaration of a DataStructure
 * # belongs to that data structure.
 *
 * [DataStructure Name1]
 *
 * DataColor    : [RGB in hex]  # Color of newly created Data elements ( Nodes )
 * Pointercolor : [RBG in Hex]  # Color of newly created Data elements ( Nodes )
 * name         : [string]      # Name of the data structure acessible in the scripting interface.
 *
 * visibility : [bool]           # is this DataStructure visible?
 *
 * ShowNamesInData      : [bool] # should the canvas show the name of the data on the screen?
 * ShowNamesInNPointers : [bool] # ↑
 * ShowValuesInData     : [bool] # ↑
 * ShowValuesInPointers : [bool] # ↑
 *
 * PluginDefinedProperty1 : propertyValue; # the plugins can define other properties for the data structure.
 * PluginDefinedProperty1 : propertyValue;
 * PluginDefinedProperty1 : propertyValue;
 *
 * UserDefinedProperty1 : propertyValue1 # the user can define other properties for the data structure.
 * UserDefinedProperty2 : propertyValue2
 * UserDefinedProperty3 : propertyValue3
 *
 * [Data 1]
 * type  : [int]
 * x     : [real]
 * y     : [real]
 * color : [string in "0xFFFFFF" format]
 * name  : [string]
 * value : [string]
 * size  : [float]
 *
 * property1 : propertyValue1
 * property2 : propertyValue2
 * property3 : propertyValue3
 *
 * [Data 2] <- same as above.
 * type  : [int]
 * x     : [integer]
 * y     : [integer]
 * color : [string in "0xFFFFFF" format]
 * name  : [string]
 * value : [string]
 * size  : [float]
 *
 * property1 : propertyValue1
 * property2 : propertyValue2
 * property3 : propertyValue3
 *
 * [Pointer 1 -> 2]
 * type     : [int]
 * name     : [string]
 * value    : [string]
 * style    : [string]
 * color    : [string in "0xFFFFFF" format]
 * width    : [float]
 *
 * property1 : propertyValue1
 * property2 : propertyValue2
 * property3 : propertyValue3
 */

bool Document::saveAsInternalFormat(const QString& filename)
{
    d->_buffer.clear();

    KSaveFile saveFile(!filename.endsWith(".graph") ? QString("%1.graph").arg(filename) : filename);

    if (!saveFile.open()) {
        kDebug() << "Error: File Not Open";
        return false;
    }

    QTextStream stream(&saveFile);
    stream.setCodec("UTF-8");

    d->_buffer = QString("[Document Properties] \n")
              % QString("top : ") % QString::number(top()) % QChar('\n')
              % QString("bottom : ") % QString::number(bottom()) % QChar('\n')
              % QString("left : ") % QString::number(left()) % QChar('\n')
              % QString("right : ") % QString::number(right()) % QChar('\n')
              % QString("DataStructurePlugin : ") % DataStructurePluginManager::self()->actualPlugin()->internalName() % QChar('\n')
              % QChar('\n');

    foreach(int dataTypeIdentifier, dataTypeList()) {
        d->_buffer += QString("[DataType %1]").arg(QString::number(dataTypeIdentifier)) % QChar('\n')
            % QString("Name : ") % dataType(dataTypeIdentifier)->name() % QChar('\n')
            % QString("IconName : ") % dataType(dataTypeIdentifier)->iconName() % QChar('\n')
            % QString("Color : ") % dataType(dataTypeIdentifier)->defaultColor().name() % QChar('\n')
            % QChar('\n');
    }

    foreach(int pointerTypeIdentifier, pointerTypeList()) {
        d->_buffer += QString("[PointerType %1]").arg(QString::number(pointerTypeIdentifier)) % QChar('\n')
            % QString("Name : ") % dataType(pointerTypeIdentifier)->name() % QChar('\n')
            % QString("Color : ") % dataType(pointerTypeIdentifier)->defaultColor().name() % QChar('\n')
            % QChar('\n');
    }

    // iterate over all data structures
    QList<DataStructurePtr>::const_iterator dataStructure = d->_dataStructures.constBegin();
    int identifier=0;
    while (dataStructure != d->_dataStructures.constEnd()) {
        d->_buffer += QString("[DataStructure %1] \n").arg(identifier++).toUtf8();

        savePropertiesInternalFormat(dataStructure->get());

        foreach(int type, dataTypeList()) {
        foreach(DataPtr n, (*dataStructure)->dataList(type)) {
            d->_buffer += QString("[Data %1]\n").arg(QString::number(n->identifier()));
            d->_buffer += QString("type : ") % QString::number(n->dataType()) % QChar('\n');
            savePropertiesInternalFormat(n.get());
        }
        }

        foreach(int type, pointerTypeList()) {
        foreach(PointerPtr e, (*dataStructure)->pointers(type)) {
            d->_buffer += QString("[Pointer %1->%2]\n").
                arg(QString::number(e->from()->identifier())).
                arg(QString::number(e->to()->identifier())).toUtf8();
            d->_buffer += QString("type : ") % QString::number(e->pointerType()) % QChar('\n');
            savePropertiesInternalFormat(e.get());
        }
        }
        ++dataStructure;
    }
    kDebug() << "------- /// BEGIN internal file format /// -------";
    kDebug() << d->_buffer;
    kDebug() << "------- /// internal file format END /// -------";

    stream << d->_buffer;

    if (!saveFile.finalize()) {
        kDebug() << "Error, file not saved.";
        return false;
    }
    d->_lastSavedDocumentPath = filename;
    d->_modified = false;
    return true;
}

void Document::savePropertiesInternalFormat(QObject *o)
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

        d->_buffer +=  QString("%1 : %2 \n").arg(name, value.toString());
    }

    QList<QByteArray> propertyNames = o->dynamicPropertyNames();
    foreach(const QByteArray & name, propertyNames) {
        QVariant value = o->property(name);
        d->_buffer +=  QString("%1 : %2 \n").arg(name, value.toString()).toUtf8();
    }

    d->_buffer += '\n';
}

void Document::loadFromInternalFormat(const KUrl& fileUrl)
{
    // delete existing data structures to create clean import
    QList<DataStructurePtr>::const_iterator iter = dataStructures().constBegin();
    while (iter != dataStructures().constEnd()) {
        remove(*iter);
        ++iter;
    }

    QFile f(fileUrl.toLocalFile());
    d->_lastSavedDocumentPath = fileUrl.toLocalFile();
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        kDebug() << "Could not load file in write mode: " << fileUrl.toLocalFile().toUtf8();
        return;
    }
    DataStructurePtr tmpDataStructure;
    QObject *tmpObject = 0;
    QMap<int, DataPtr> dataMap;

    QTextStream in(&f);
    in.setCodec("UTF-8");

    while (!in.atEnd()) {
        QString str = in.readLine().simplified();

        if (str.startsWith('#')) { //! Ignore it, commented line.
            continue;
        } else if (str.startsWith(QLatin1String("[Document Properties]"))) {

            QString dataLine = in.readLine().simplified();
            while (!in.atEnd() && !dataLine.isEmpty()) {
                /**/ if (dataLine.startsWith(QLatin1String("top :")))      setTop(dataLine.section(' ', 2).toInt());
                else if (dataLine.startsWith(QLatin1String("bottom :")))   setBottom(dataLine.section(' ', 2).toInt());
                else if (dataLine.startsWith(QLatin1String("left :")))     setLeft(dataLine.section(' ', 2).toInt());
                else if (dataLine.startsWith(QLatin1String("right :")))    setRight(dataLine.section(' ', 2).toInt());
                else if (dataLine.startsWith(QLatin1String("DataStructurePlugin :"))) {
                    // set plugin by unique plugin identifier
                    QString pluginIdentifier = dataLine.section(' ', 2);
                    DataStructurePluginManager::self()->setDataStructurePlugin(pluginIdentifier);
                    d->_dataStructureType = DataStructurePluginManager::self()->actualPlugin();
                } else if (!dataLine.isEmpty())               break; // go to the last if and finish populating.
                dataLine = in.readLine().simplified();
            }
            tmpObject = this;
        } else if (str.startsWith(QLatin1String("[DataStructure"))) {
            QString gName = str.section(' ', 1, 1);
            gName.remove(']');
            tmpDataStructure = DataStructurePluginManager::self()->createNewDataStructure(this);
            tmpDataStructure->setName(gName.toAscii());
            d->_dataStructures.append(tmpDataStructure);
            tmpObject = tmpDataStructure.get();
        }

        else if (str.startsWith(QLatin1String("[DataType"))) {
            QString identifier = str.section(' ', 1);
            identifier.remove(']');
            int tmpDataTypeId = registerDataType(QString(), identifier.toInt());
            DataTypePtr tmpDataType = dataType(tmpDataTypeId);

            QString dataLine = in.readLine().simplified();
            while (!in.atEnd() && !dataLine.isEmpty()) {
                /**/ if (dataLine.startsWith(QLatin1String("Name :")))      tmpDataType->setName(dataLine.section(' ', 2));
                else if (dataLine.startsWith(QLatin1String("IconName :")))  tmpDataType->setIcon(dataLine.section(' ', 2));
                else if (dataLine.startsWith(QLatin1String("Color :")))     tmpDataType->setDefaultColor(QColor(dataLine.section(' ', 2)));
                else if (!dataLine.isEmpty())               break;  // go to the last if and finish populating.
                dataLine = in.readLine().simplified();
            }
        }

        else if (str.startsWith(QLatin1String("[PointerType"))) {
            QString identifier = str.section(' ', 1);
            identifier.remove(']');
            int tmpPointerTypeId = registerPointerType(QString(), identifier.toInt());
            PointerTypePtr tmpPointerType = pointerType(tmpPointerTypeId);

            QString dataLine = in.readLine().simplified();
            while (!in.atEnd() && !dataLine.isEmpty()) {
                /**/ if (dataLine.startsWith(QLatin1String("Name :")))      tmpPointerType->setName(dataLine.section(' ', 2));
                else if (dataLine.startsWith(QLatin1String("Color :")))     tmpPointerType->setDefaultColor(QColor(dataLine.section(' ', 2)));
                else if (!dataLine.isEmpty())               break;  // go to the last if and finish populating.
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
            if (dataTypeList().contains(type)) {
                tmpData = tmpDataStructure->addData(name, type);
            } else {
                kDebug() << "Create data element of type 0, since type " << type << " was not registered.";
                tmpData = tmpDataStructure->addData(name, 0);
            }
            tmpData->setValue(value);
            tmpData->setColor(color);
            tmpData->setPos(posX, posY);

            // add to data element map
            QString identifier = str.section(' ', 1);
            identifier.remove(']');
            dataMap.insert(identifier.toInt(), tmpData);
            tmpObject = tmpData.get();
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
            QString style = "";
            while (!in.atEnd() && !dataLine.isEmpty()) {
                /**/ if (dataLine.startsWith(QLatin1String("width :")))     width = dataLine.section(' ', 2).toInt();
                else if (dataLine.startsWith(QLatin1String("value :")))     value = dataLine.section(' ', 2);
                else if (dataLine.startsWith(QLatin1String("type :")))      type = dataLine.section(' ', 2).toInt();
                else if (dataLine.startsWith(QLatin1String("color :")))     color = dataLine.section(' ', 2);
                else if (dataLine.startsWith(QLatin1String("style :")))     style = dataLine.section(' ', 2);
                else if (!dataLine.isEmpty())                break;  // go to the last if and finish populating.
                dataLine = in.readLine().simplified();
            }
            if (pointerTypeList().contains(type)) {
                tmpPointer = tmpDataStructure->addPointer(dataMap[nameFrom.toInt()],dataMap[nameTo.toInt()], type);
            } else {
                kDebug() << "Create pointer of type 0, since type " << type << " was not registered.";
                tmpPointer = tmpDataStructure->addPointer(dataMap[nameFrom.toInt()],dataMap[nameTo.toInt()], 0);
            }
            tmpPointer->setWidth(width);
            tmpPointer->setValue(value);
            tmpPointer->setColor(color);
            tmpPointer->setStyle(style);
            tmpObject = tmpPointer.get();
        }

        else if (str.startsWith(QLatin1String("[Group"))) {
            /*QString gName = str.section(" ",1,1);
            gName.remove(']');
            tmpGroup = tmpDataStructure->addGroup(gName); */
        }

        else if (str.contains(':')) {
            QString propertyName = str.section(':', 0, 0).trimmed();
            QString propertyValue = str.section(':', 1, 1).trimmed();
            tmpObject->setProperty(propertyName.toUtf8() , propertyValue);
        }
    }
    d->_modified = false;
}

DataStructurePtr Document::activeDataStructure() const
{
    return d->_activeDataStructure;
}

QString Document::dataStructureTypeName() const
{
    return d->_dataStructureType->name();
}

QString Document::dataStructureInternalName() const
{
    return  d->_dataStructureType->internalName();
}


DataStructurePluginInterface* Document::dataStructurePlugin() const
{
    return d->_dataStructureType;
}

// void Document::convertToDataStructure(QString newDataStructure){
//     if (newDataStructure == d->_dataStructureType) return;
// //        kDebug() << "Need convert doc from " << d->_dataStructureType << " to "<< newDataStructure ;
//         d->_dataStructureType = newDataStructure;
// //         Document * gDoc = new Document(*this);
// //        int numDataStructures = count();
// //         for (int i = 0 ; i < numDataStructures; ++i){
// //             DataStructure * g = DataStructurePluginManager::instance()->changeToDataStructure(at(i));
// //             if (at(i) == d->_activeDataStructure)
// //               d->_activeDataStructure = g;
// //             append(g);
// //         }
// //         for (int i = 0 ; i < numDataStructures; ++i){
// //             at(0)->deleteLater();
// //             removeAt(0);
// //         }
// //         return gDoc;
// //    }
// //     return 0;
// }
