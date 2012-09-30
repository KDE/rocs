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
#include <boost/concept_check.hpp>

#include "DataStructurePluginManager.h"
#include "DataStructurePluginInterface.h"
#include <KStandardDirs>

class DocumentPrivate
{
public:
    DocumentPrivate() {}

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
    DataTypePtr _groupType;

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

// FIXME
//     d->_iconPackage = KGlobal::dirs()->locate("appdata", "iconpacks/default.svg");
//     if (!DataItem::sharedRenderer(d->_iconPackage)) {
//          DataItem::registerSharedRenderer(d->_iconPackage);
//     }

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

//FIXME
//     d->_iconPackage = gd.iconPackage();
//     if (!DataItem::sharedRenderer(d->_iconPackage)) {
//          DataItem::registerSharedRenderer(d->_iconPackage);
//     }

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

int Document::groupType()
{
    if (!d->_groupType) {
        int typeId = registerDataType("Subdatastructure");
        d->_groupType = dataType(typeId);
        return typeId;
    }
    return d->_groupType->identifier();
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
//FIXME
//     int elements = dataStructures().size();
//     for (int i = 0; i < elements; i++) {
//         bool resizeDocument = false;
//         foreach(DataPtr n,  dataStructures().at(i)->dataList()) {
//             if (n->x() < d->_left + GraphScene::kBORDER) {
//                 setLeft(d->_left - GraphScene::kBORDER);
//                 resizeDocument = true;
//             }
//             if (n->x() > d->_right - GraphScene::kBORDER) {
//                 setRight(d->_right + GraphScene::kBORDER);
//                 resizeDocument = true;
//             }
//             if (n->y() < d->_top + GraphScene::kBORDER) {
//                 setTop(d->_top - GraphScene::kBORDER);
//                 resizeDocument = true;
//             }
//             if (n->y() > d->_bottom - GraphScene::kBORDER) {
//                 setBottom(d->_bottom + GraphScene::kBORDER);
//                 resizeDocument = true;
//             }
//         }
//         if (resizeDocument) {
//             emit resized();
//         }
//     }
}

void Document::resizeDocumentBorder(Document::Border orientation)
{
//FIXME
//     bool empty = true;
//     int elements = dataStructures().size();
//
//     // scans doubled border of specified size: if empty or not
//     for (int i = 0; i < elements; i++) {
//         foreach(DataPtr n,  dataStructures().at(i)->dataList()) {
//             switch (orientation) {
//             case BorderLeft: {
//                 if (n != 0 && n->x() < d->_left + GraphScene::kBORDER * 2) empty = false;
//                 break;
//             }
//             case BorderRight: {
//                 if (n != 0 && n->x() > d->_right - GraphScene::kBORDER * 2) empty = false;
//                 break;
//             }
//             case BorderTop: {
//                 if (n != 0 && n->y() < d->_top + GraphScene::kBORDER * 2) empty = false;
//                 break;
//             }
//             case BorderBottom: {
//                 if (n != 0 && n->y() > d->_bottom - GraphScene::kBORDER * 2) empty = false;
//                 break;
//             }
//             }
//         }
//     }
//
//     //TODO connect to graphvisualeditor-size
//     if (empty == true) {
//         switch (orientation) {
//         case BorderLeft: {
//             if (d->_right - d->_left < GraphScene::kBORDER * 4)
//                 return;
//             setLeft(d->_left + GraphScene::kBORDER);
//             emit resized();
//             break;
//         }
//         case BorderRight: {
//             if (d->_right - d->_left < GraphScene::kBORDER * 4)
//                 return;
//             setRight(d->_right - GraphScene::kBORDER);
//             emit resized();
//             break;
//         }
//         case BorderTop: {
//             if (d->_bottom - d->_top < GraphScene::kBORDER * 4)
//                 return;
//             setTop(d->_top + GraphScene::kBORDER);
//             emit resized();
//             break;
//         }
//         case BorderBottom: {
//             if (d->_bottom - d->_top < GraphScene::kBORDER * 4)
//                 return;
//             setBottom(d->_bottom - GraphScene::kBORDER);
//             emit resized();
//             break;
//         }
//         }
//     }
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

void Document::setActiveDataStructure(int index)
{
    if (index >= 0 && index < d->_dataStructures.length()) {
        d->_activeDataStructure = d->_dataStructures.at(index);
        emit activeDataStructureChanged(d->_activeDataStructure);
        d->_modified = true;
    }
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
    emit dataStructureListChanged();
    return g;
}

void Document::save()
{
    Q_ASSERT(!fileUrl().isEmpty());
    DocumentManager::self()->saveDocumentAs(this, KUrl::fromLocalFile(fileUrl()));
}

void Document::saveAs(const QString& fileUrl)
{
    DocumentManager::self()->saveDocumentAs(this, KUrl::fromLocalFile(fileUrl));
}

const QString& Document::fileUrl() const
{
    return d->_lastSavedDocumentPath;
}

void Document::setFileUrl(KUrl fileUrl)
{
    d->_lastSavedDocumentPath = fileUrl.toLocalFile();
}

void Document::remove(DataStructurePtr dataStructure)
{
    d->_dataStructures.removeOne(dataStructure);
    if (d->_dataStructures.count() == 0) {
        addDataStructure();
    }
    setActiveDataStructure(d->_dataStructures.count()-1);
    d->_modified = true;
    emit dataStructureListChanged();
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
    return d->_dataStructureType->internalName();
}

DataStructurePluginInterface* Document::dataStructurePlugin() const
{
    return d->_dataStructureType;
}

void Document::setDataStructurePlugin(QString pluginIdentifier)
{
    DataStructurePluginInterface* plugin = DataStructurePluginManager::self()->plugin(pluginIdentifier);
    if (plugin) {
        d->_dataStructureType = plugin;
    }
}
