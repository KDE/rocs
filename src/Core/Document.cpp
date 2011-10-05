/* 
    This file is part of Rocs.
    Copyright 2008-2011  Tomaz Canabrava <tomaz.canabrava@gmail.com>
    Copyright 2008       Ugo Sangiori <ugorox@gmail.com>
    Copyright 2010-2011  Wagner Reck <wagner.reck@gmail.com>
    Copyright 2011       Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#include <boost/shared_ptr.hpp>

//#include "dataStructureGroups.h"

#include "DataStructurePluginManager.h"
#include <GraphScene.h>
#include "DataStructurePluginInterface.h"

class DocumentPrivate{
public:
    DocumentPrivate(){}

    QString _buf;
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


};

Document::Document(const QString& name, qreal left, qreal right, qreal top, qreal bottom, QObject *parent)
        : QObject(parent), d( new DocumentPrivate())
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

    for (int i = 0; i < gd.dataStructures().count(); ++i){
        d->_dataStructures.append(
            DataStructurePluginManager::self()->changeToDataStructure(
                gd.d->_dataStructures.at(i),this
            )
        );
    }
}

// Default Destructor
Document::~Document() {
    for(int i = 0; i < d->_dataStructures.size(); i ++){
        d->_dataStructures.clear();
    }
}

QList< DataStructurePtr >& Document::dataStructures() const { return d->_dataStructures; }

QtScriptBackend * Document::engineBackend() const{
    return d->_engineBackend;
}

// Sets the current file name of the DataStructure Collection
void Document::setName(const QString& name) {
    d->_name = name;
    d->_modified = true;
    emit nameChanged(name);
}

// Gets the name of the DataStructure
QString Document::name() const {
    return d->_name;
}

// gets the wheight of the drawable area
qreal Document::height() const {
    return d->_bottom - d->_top;
}

// sets the width of the drawable area
qreal Document::width() const {
    return d->_right - d->_left;
}

qreal Document::left() const {
    return d->_left;
}

qreal Document::right() const {
    return d->_right;
}

qreal Document::top() const {
    return d->_top;
}

qreal Document::bottom() const {
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
    return QRectF(d->_left, d->_top,d->_right - d->_left, d->_bottom - d->_top );
}

bool Document::isPointAtDocument(qreal x, qreal y)  const {

    if (x < d->_left)      return false;
    if (x > d->_right)     return false;
    if (y < d->_top)       return false;
    if (y > d->_bottom)    return false;

    return true;
}

void Document::changeMinimalSize(qreal width, qreal height) {
    if (width>=0) d->_minWidth = width;
    if (height>=0) d->_minHeight = height;

    if (width > d->_right - d->_left) {
        d->_left -= (d->_right- d->_left - width)/2;
        d->_right += (d->_right- d->_left - width)/2;
        emit resized();
    }
    else {
        resizeDocumentBorder(BorderLeft);
    }

    if (height > d->_bottom - d->_top) {
        d->_top -= (d->_bottom - d->_top - height)/2;
        d->_bottom += (d->_bottom - d->_top - height)/2;
        emit resized();
    }
    else {
        resizeDocumentBorder(BorderTop);
    }
}

void Document::resizeDocumentIncrease()
{
    int elements = dataStructures().size();
    for (int i = 0; i < elements; i++) {
        bool resizeDocument = false;
        foreach( DataPtr n,  dataStructures().at(i)->dataList() ){
            if (n->x() < d->_left+GraphScene::kBORDER) {
                setLeft(d->_left-GraphScene::kBORDER);
                resizeDocument = true;
            }
            if (n->x() > d->_right-GraphScene::kBORDER) {
                setRight(d->_right+GraphScene::kBORDER);
                resizeDocument = true;
            }
            if (n->y() < d->_top+GraphScene::kBORDER) {
                setTop(d->_top-GraphScene::kBORDER);
                resizeDocument = true;
            }
            if (n->y() > d->_bottom-GraphScene::kBORDER) {
                setBottom(d->_bottom+GraphScene::kBORDER);
                resizeDocument = true;
            }
        }
        if (resizeDocument) {
            emit resized();
        }
    }
}

void Document::resizeDocumentBorder(Document::Border orientation) {
    bool empty = true;
    int elements = dataStructures().size();

    // scans doubled border of specified size: if empty or not
    for (int i = 0; i < elements; i++) {
    foreach( DataPtr n,  dataStructures().at(i)->dataList() ){
        switch (orientation) {
        case BorderLeft: {
            if (n!=0 && n->x() < d->_left+GraphScene::kBORDER*2) empty=false;
            break;
        }
        case BorderRight: {
            if (n!=0 && n->x() > d->_right-GraphScene::kBORDER*2) empty=false;
            break;
        }
        case BorderTop: {
            if (n!=0 && n->y() < d->_top+GraphScene::kBORDER*2) empty=false;
            break;
        }
        case BorderBottom: {
            if (n!=0 && n->y() > d->_bottom-GraphScene::kBORDER*2) empty=false;
            break;
        }
        }
    }
    }

    //TODO connect to graphvisualeditor-size
    if (empty==true) {
        switch (orientation) {
        case BorderLeft: {
            if (d->_right-d->_left < GraphScene::kBORDER*4)
                return;
            setLeft(d->_left+GraphScene::kBORDER);
            emit resized();
            break;
        }
        case BorderRight: {
            if (d->_right-d->_left < GraphScene::kBORDER*4)
                return;
            setRight(d->_right-GraphScene::kBORDER);
            emit resized();
            break;
        }
        case BorderTop: {
            if (d->_bottom-d->_top < GraphScene::kBORDER*4)
                return;
            setTop(d->_top+GraphScene::kBORDER);
            emit resized();
            break;
        }
        case BorderBottom: {
            if (d->_bottom-d->_top < GraphScene::kBORDER*4)
                return;
            setBottom(d->_bottom-GraphScene::kBORDER);
            emit resized();
            break;
        }
        }
    }
}

bool Document::isPointAtDocument(QPointF point)  const {
    return isPointAtDocument(point.x(), point.y());
}

bool Document::isModified() const{
    return d->_modified;
}

void Document::cleanUpBeforeConvert()
{
    foreach (DataStructurePtr ds, d->_dataStructures)
        ds->cleanUpBeforeConvert();
}


void Document::setActiveDataStructure(DataStructurePtr g){
    if (d->_dataStructures.indexOf(g) != -1){
        d->_activeDataStructure = g;
        emit activeDataStructureChanged(g);
        d->_modified = true;
    }
}

DataStructurePtr Document::addDataStructure(QString name) {
    DataStructurePtr g = DataStructurePluginManager::self()->createNewDataStructure(this,
                                                                                  d->_dataStructureType->name());
    g->setName(name);
    d->_dataStructures.append(g);
    d->_activeDataStructure = g;
    d->_modified = true;
    connect(g.get(), SIGNAL(changed()), this, SLOT(setModified()));
    emit dataStructureCreated(g);
    return g;
}

void Document::savedDocumentAt(const QString& fileName) {
    d->_lastSavedDocumentPath = fileName;
}

const QString& Document::documentPath() const {
    return d->_lastSavedDocumentPath;
}

void Document::remove(DataStructurePtr dataStructure){
    d->_dataStructures.removeOne(dataStructure);
    qDebug() << "Document::remove(Datastructure)";
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
 * # Data Structre Definitions.
 * # Data Structures are layered, so there can be N of them.
 * # Every Data and Pointer below a declaration of a DataStructure
 * # belongs to that data structure.
 * 
 * [DataStructure Name1] 
 * 
 * DataColor    : [RGB in hex]  # Color of newly created Datas ( Nodes )
 * Pointercolor : [RBG in Hex]  # Color of newly created Datas ( Nodes )
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
 * [Data 2] <- same as above.
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

 bool Document::saveAsInternalFormat(const QString& filename) {
    d->_buf.clear();

    KSaveFile saveFile( !filename.endsWith(".graph") ? QString("%1.graph").arg(filename) : filename);

    if (!saveFile.open()) {
        kDebug() << "Error: File Not Open";
        return false;
    }

    QTextStream stream(&saveFile);
    stream.setCodec("UTF-8");
    
     d->_buf = QString("[Document Properties] \n")
             % QString("Width : ") % QString::number(width()) % QChar('\n')
             % QString("Height : ") % QString::number(height()) % QChar('\n')
             % QString("DataStructurePlugin : ") % DataStructurePluginManager::self()->actualPlugin()->name() % QChar('\n')
             % QChar('\n');
            
    for (int i = 0; i < d->_dataStructures.count(); i++) {
        DataStructurePtr g = d->_dataStructures.at(i);

        d->_buf += QString("[DataStructure %1] \n").arg(i).toUtf8();

        savePropertiesInternalFormat(g.get());

        foreach( DataPtr n, g->dataList()) {
            d->_buf += QString("[Data %1]\n").arg(g->dataList().indexOf(n)).toUtf8();
            savePropertiesInternalFormat(n.get());
        }

        int from, to;
        foreach( PointerPtr e, g->pointers()) {
            from = g->dataList().indexOf(e->from());
            to = g->dataList().indexOf(e->to());
            d->_buf += QString("[Pointer %1->%2]\n").arg(from).arg(to).toUtf8();
            savePropertiesInternalFormat(e.get());
        }

        /*     buf += " \n \n ############ GROUPS ########### \n \n";
             foreach( DataStructureGroup *gg, g->groups()) {
                 buf += QString("[Group %1] \n").arg((long) gg);

                 foreach( ::Data *n, gg->data() ) {
                     buf += QString("%1\n").arg((long)n);
                 }
             } */
    }
    kDebug() << d->_buf;

    stream << d->_buf;

    if (!saveFile.finalize()) {
        kDebug() << "Error, file not saved.";
        return false;
    }
    d->_lastSavedDocumentPath = filename;
    d->_modified = false;
    return true;
}

void Document::savePropertiesInternalFormat(QObject *o) {
    const QMetaObject *metaObject = o->metaObject();
    int propertyCount = metaObject->propertyCount();

    for ( int i = 0; i < propertyCount; ++i) {
        QMetaProperty metaProperty = metaObject->property(i);
        const char *name = metaProperty.name();
        QVariant value = o->property(name);

        if ( QString("objectName").compare(metaProperty.name()) == 0) {
            continue;
        }
        else if( QString("name").compare(metaProperty.name()) == 0 ) {
          QString namevalue = QString("%1 : %2 \n" ).arg(name).arg(value.toString());
        }

        d->_buf +=  QString("%1 : %2 \n" ).arg(name, value.toString());
    }

    QList<QByteArray> propertyNames = o->dynamicPropertyNames();
    foreach(const QByteArray& name, propertyNames) {
        QVariant value = o->property(name);
        d->_buf +=  QString("%1 : %2 \n" ).arg(name, value.toString()).toUtf8();
    }

    d->_buf += '\n';
}

void Document::loadFromInternalFormat(const QString& filename) {
    QFile f(filename);
    if ( !f.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        qDebug() << "File not open " << filename.toUtf8();
        return;
    }
    DataStructurePtr tmpDataStructure;
    QObject *tmpObject = 0;
    DataPtr tmpDataPtr;

    QTextStream in(&f);
    in.setCodec("UTF-8");

    while (!in.atEnd()) {
        QString str = in.readLine().simplified();

        if (str.startsWith('#')) { //! Ignore it, commented line.
            continue;
        }
        else if(str.startsWith("[Document Properties]")){
            tmpObject = this;
        }
        else if (str.startsWith("[DataStructure")) {
            QString gName = str.section(' ',1,1);
            gName.remove(']');
            tmpDataStructure = DataStructurePluginManager::self()->createNewDataStructure(this);
            tmpDataStructure->setName(gName.toAscii());
            d->_dataStructures.append(tmpDataStructure);
            tmpObject = tmpDataStructure.get();
        }

        else if (str.startsWith("[Data")) {
            tmpDataPtr = tmpDataStructure->addData(QString());
            QString dataLine = in.readLine().simplified();
            qreal posX = 0;
            qreal posY = 0;
            while (!in.atEnd() && !dataLine.isEmpty()) {
                if (dataLine.startsWith("x :")) {
                    posX = dataLine.section(' ',2).toInt();
                    qDebug() << "Setting X " << posX;
                }
                if (dataLine.startsWith("y :")){
                    posY = dataLine.section(' ',2).toInt();
                    qDebug() << "Setting Y " << posY;
                }
                if (dataLine.startsWith("value :"))
                    tmpDataPtr->setValue(dataLine.section(' ',2).toInt());
                if (dataLine.startsWith("color :"))
                    tmpDataPtr->setColor(dataLine.section(' ',2));
                if (dataLine.startsWith("name :")){
                    tmpDataPtr->setName(dataLine.section(' ', 2));
                }
                dataLine = in.readLine().simplified();
            }
            tmpDataPtr->setPos(posX, posY);
            qDebug() << tmpDataPtr->x() << tmpDataPtr->y();
            tmpObject = tmpDataPtr.get();
        }

        else if (str.startsWith("[Pointer")) {
            QString eName = str.section(' ',1,1);
            eName.remove(']');

            QString nameFrom = eName.section("->", 0,0);
            QString nameTo = eName.section("->", 1,1);

            PointerPtr tmpPointer = tmpDataStructure->addPointer(tmpDataStructure->dataList().at(nameFrom.toInt()),
                                                     tmpDataStructure->dataList().at(nameTo.toInt()));
            QString dataLine = in.readLine().simplified();
            while (!in.atEnd() && !dataLine.isEmpty()) {
                if (dataLine.startsWith("width :"))
                    tmpPointer->setWidth(dataLine.section(' ',2).toInt());
                if (dataLine.startsWith("value :"))
                    tmpPointer->setValue(dataLine.section(' ',2));
                dataLine = in.readLine().simplified();
            }
            tmpObject = tmpPointer.get();
        }
        else if (str.startsWith("[Group")) {
            /*QString gName = str.section(" ",1,1);
            gName.remove(']');
            tmpGroup = tmpDataStructure->addGroup(gName); */
        }
        else if (str.contains(':')) {
            QString propertyName = str.section(':',0,0).trimmed();
            QString propertyValue = str.section(':',1,1).trimmed();
            tmpObject->setProperty( propertyName.toUtf8() , propertyValue );
        }
        //else {
        //      tmpGroup->append( tmpDataStructure->data(str));
        //}
    }
    d->_modified = false;
    qDebug() << "DataStructure Document Loaded.";
}

DataStructurePtr Document::activeDataStructure() const { return d->_activeDataStructure; }

QString Document::dataStructureTypeName() const
{
  return d->_dataStructureType->name();
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
