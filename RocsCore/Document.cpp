/*
    This file is part of Rocs.
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>
    Copyright 2010-2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2011-2013  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
#include "Data.h"
#include "Pointer.h"
#include "PointerType.h"
#include "DataType.h"
#include "DataStructureBackendManager.h"
#include "DataStructureBackendInterface.h"
#include "QtScriptBackend.h"
#include "ConcurrentHelpClasses.h"

#include <KStandardDirs>
#include <KSaveFile>
#include <KDebug>

#include <QString>
#include <QByteArray>
#include <QSvgRenderer>
#include <QMap>

class DocumentPrivate
{
public:
    DocumentPrivate()
        : _left(0)
        , _right(0)
        , _top(0)
        , _bottom(0)
    { }

    enum borderSize { SceneBorder = 50 };

    QString _lastSavedDocumentPath;
    QString _name;
    qreal _left;
    qreal _right;
    qreal _top;
    qreal _bottom;
    bool _modified;
    DataStructurePtr _activeDataStructure;
    QPointer<DataStructureBackendInterface> _backend;
    QtScriptBackend* _engineBackend;
    QList< DataStructurePtr > _dataStructures;

    // data and pointer types
    QMap<int, DataTypePtr> _dataTypes;           // list of data types
    QMap<int, PointerTypePtr> _pointerTypes;        // list of pointer types
    DataTypePtr _groupType;

    QString _iconPackage;   // available icons for data types
};



Document::Document(const QString& name, QObject *parent)
    : QObject(parent)
    , d(new DocumentPrivate())
{
    d->_name = name;
    d->_engineBackend = new QtScriptBackend(this);
    d->_backend = DataStructureBackendManager::self().activeBackend();
    d->_modified = false;

    d->_iconPackage = KGlobal::dirs()->locate("appdata", "iconpacks/default.svg");
    if (!DocumentManager::self().sharedRenderer(d->_iconPackage)) {
         DocumentManager::self().registerSharedRenderer(d->_iconPackage);
    }

    // default types
    d->_dataTypes.insert(0, DataType::create(this, 0));
    d->_pointerTypes.insert(0, PointerType::create(this, 0));

    kDebug() << "Construct Graph Document of type : " << d->_backend->name();
}

void Document::setModified(const bool mod)
{
    d->_modified = mod;
}

void Document::changeBackend()
{
    cleanUpBeforeConvert();
    d->_backend = DataStructureBackendManager::self().activeBackend();

    // create list of existing data structures, then convert them one by one
    QList<DataStructurePtr> dataStructures = QList<DataStructurePtr>(d->_dataStructures);
    d->_dataStructures.clear();

    for (int i = 0; i < dataStructures.count(); ++i) {
        DataStructurePtr newDataStructure = addDataStructure(
                DataStructureBackendManager::self().createDataStructure(dataStructures.at(i), this));
        // remove origin data structure
        dataStructures[i]->remove();
    }
}

Document::~Document()
{
    clear();
    delete d->_engineBackend;
}

int Document::registerDataType(const QString& name, int identifier)
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

int Document::registerPointerType(const QString& name, int identifier)
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

QList<int> Document::dataTypeList() const
{
    return d->_dataTypes.keys();
}

QList<int> Document::pointerTypeList() const
{
    return d->_pointerTypes.keys();
}

QString Document::iconPackage() const
{
    return d->_iconPackage;
}

bool Document::removeDataType(int identifier)
{
    Q_ASSERT(d->_dataTypes.contains(identifier));

    emit(dataTypeRemoved(identifier));
    if (d->_dataTypes.contains(identifier)) {
        d->_dataTypes[identifier]->remove();
    }
    return d->_dataTypes.remove(identifier) > 0;
}

bool Document::removePointerType(int identifier)
{
    Q_ASSERT(d->_pointerTypes.contains(identifier));

    emit(pointerTypeRemoved(identifier));
    if (d->_pointerTypes.contains(identifier)) {
        d->_pointerTypes[identifier]->remove();
    }
    return d->_pointerTypes.remove(identifier) > 0;
}

DataTypePtr Document::dataType(int identifier) const
{
    Q_ASSERT(d->_dataTypes.contains(identifier));

    if (!d->_dataTypes.contains(identifier)) {
        return DataTypePtr();
    }
    return d->_dataTypes[identifier];
}

PointerTypePtr Document::pointerType(int identifier) const
{
    Q_ASSERT(d->_pointerTypes.contains(identifier));

    if (!d->_pointerTypes.contains(identifier)) {
        return PointerTypePtr();
    }
    return d->_pointerTypes[identifier];
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

bool Document::isValidIdentifier(const QString& identifier)
{
    QRegExp validator("(^([a-z]|[A-Z])+([0-9]|[a-z]|[A-Z]|_)*$)");
    return (validator.indexIn(identifier) != -1);
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

void Document::updateGraphics(DataTypePtr dataType)
{
    foreach(const DataStructurePtr &structure, d->_dataStructures) {
        QtConcurrent::blockingMap(structure->dataList(dataType->identifier()), DataGraphicsUpdated());
    }
}

void Document::updateGraphics(PointerTypePtr pointerType)
{
    foreach(const DataStructurePtr &structure, d->_dataStructures) {
        QtConcurrent::blockingMap(structure->pointers(pointerType->identifier()), PointerGraphicsUpdated());
    }
}

QRectF Document::sceneRect() const
{
    return QRectF(d->_left, d->_top, d->_right - d->_left, d->_bottom - d->_top);
}

void Document::updateSceneRect(DataPtr newData)
{
    if (sceneRect().contains(newData->x(), newData->y()) == true) {
        //TODO implement decreasing of scene rect
        return;
    }

    QPointF coordinate = QPointF(newData->x(), newData->y());

    if (coordinate.x() < d->_left) {
        d->_left = coordinate.x();
    }
    if (coordinate.x() > d->_right) {
        d->_right = coordinate.x();
    }
    if (coordinate.y() < d->_top) {
        d->_top = coordinate.y();
    }
    if (coordinate.y() > d->_bottom) {
        d->_bottom = coordinate.y();
    }

    emit sceneRectChanged(sceneRect());
}

bool Document::isModified() const
{
    return d->_modified;
}

void Document::cleanUpBeforeConvert()
{
    foreach(DataStructurePtr ds, d->_dataStructures) {
        ds->cleanUpBeforeConvert();
    }
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

DataStructurePtr Document::addDataStructure(const QString& name)
{
    DataStructurePtr dataStructure = DataStructureBackendManager::self().createDataStructure(
            this, d->_backend->internalName());
    dataStructure->setName(name);
    return addDataStructure(dataStructure);
}

DataStructurePtr Document::addDataStructure(DataStructurePtr dataStructure)
{
    // find unused name
    QList<QString> usedNames;
    foreach(DataStructurePtr ds, d->_dataStructures) {
        usedNames.append(ds->name());
    }
    // Change unique data structure name if the currently set one is not ok.
    // For at least one i in the following range, the name is not used, yet.
    QString uniqueName = dataStructure->name();
    if (uniqueName.isEmpty() || usedNames.contains(uniqueName)) {
        for (int i = 0; i < dataStructures().length() + 1; ++i) {
            uniqueName = QString("%1%2").arg(d->_backend->internalName()).arg(i);
            if (!usedNames.contains(uniqueName)) {
                break;
            }
        }
    }

    dataStructure->setName(uniqueName);
    d->_dataStructures.append(dataStructure);
    d->_activeDataStructure = dataStructure;
    d->_modified = true;

    connect(dataStructure.get(), SIGNAL(changed()), this, SLOT(setModified()));
    connect(dataStructure.get(), SIGNAL(dataPositionChanged(DataPtr)), this, SLOT(updateSceneRect(DataPtr)));

    emit dataStructureCreated(dataStructure);
    emit dataStructureListChanged();

    return dataStructure;
}

void Document::save()
{
    Q_ASSERT(!fileUrl().isEmpty());
    DocumentManager::self().saveDocumentAs(this, KUrl::fromLocalFile(fileUrl()));
}

void Document::saveAs(const QString& fileUrl)
{
    DocumentManager::self().saveDocumentAs(this, KUrl::fromLocalFile(fileUrl));
}

QString Document::fileUrl() const
{
    return d->_lastSavedDocumentPath;
}

void Document::setFileUrl(const KUrl& fileUrl)
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

void Document::clear()
{
    // remove types
    for (QMap<int,DataTypePtr>::const_iterator iter= d->_dataTypes.constBegin(); iter != d->_dataTypes.constEnd(); ++iter) {
        emit(dataTypeRemoved(iter.key()));
        d->_dataTypes[iter.key()]->remove();
    }
    d->_dataTypes.clear();

    for (QMap<int,PointerTypePtr>::const_iterator iter= d->_pointerTypes.constBegin(); iter != d->_pointerTypes.constEnd(); ++iter) {
        emit(pointerTypeRemoved(iter.key()));
        d->_pointerTypes[iter.key()]->remove();
    }
    d->_pointerTypes.clear();

    // remove data structures
    foreach (const DataStructurePtr &dataStructure, d->_dataStructures) {
        dataStructure->remove();
    }
    d->_activeDataStructure.reset();
    d->_dataStructures.clear();
}

DataStructurePtr Document::activeDataStructure() const
{
    return d->_activeDataStructure;
}

DataStructureBackendInterface * Document::backend() const
{
    return d->_backend;
}

void Document::setBackend(const QString &pluginIdentifier)
{
    DataStructureBackendInterface * plugin = DataStructureBackendManager::self().backend(pluginIdentifier);
    if (plugin) {
        d->_backend = plugin;
    }
}
