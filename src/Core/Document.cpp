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
    qreal _xLeft;
    qreal _xRight;
    qreal _yTop;
    qreal _yBottom;
    qreal _minWidth;
    qreal _minHeight;
    bool _modified;
    bool _saved;
    DataStructure *_activeDataStructure;
    QPointer<DataStructurePluginInterface> _dataStructureType;
    QtScriptBackend* _engineBackend;
    QList<DataStructure*> _dataStructures;


};

Document::Document(const QString& name, qreal xLeft, qreal xRight, qreal yTop, qreal yBottom, QObject *parent)
        : QObject(parent), d( new DocumentPrivate())
{
    d->_name = name;
    d->_xLeft = xLeft;
    d->_xRight = xRight;
    d->_yTop = yTop;
    d->_yBottom = yBottom;
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
    d->_xLeft = gd.xLeft();
    d->_xRight = gd.xRight();
    d->_yTop = gd.yTop();
    d->_yBottom = gd.yBottom();
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
	DataStructure *g = d->_dataStructures.at(i);
        delete g;
    }
    delete d;
}

QList<DataStructure*>& Document::dataStructures() const { return d->_dataStructures; }

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
    return d->_yBottom - d->_yTop;
}

// sets the width of the drawable area
qreal Document::width() const {
    return d->_xRight - d->_xLeft;
}

qreal Document::xLeft() const {
    return d->_xLeft;
}

qreal Document::xRight() const {
    return d->_xRight;
}

qreal Document::yTop() const {
    return d->_yTop;
}

qreal Document::yBottom() const {
    return d->_yBottom;
}

void Document::setXLeft(qreal xLeftValue)
{
    d->_xLeft = xLeftValue;
    //d->_modified = true;
}

void Document::setXRight(qreal xRightValue)
{
    d->_xRight = xRightValue;
    //d->_modified = true;
}

void Document::setYTop(qreal yTopValue)
{
    d->_yTop = yTopValue;
    //d->_modified = true;
}

void Document::setYBottom(qreal yBottomValue)
{
    d->_yBottom = yBottomValue;
    //d->_modified = true;
}

QRectF Document::size()
{
    return QRectF(d->_xLeft, d->_yTop,d->_xRight - d->_xLeft, d->_yBottom - d->_yTop );
}

bool Document::isPointAtDocument(qreal x, qreal y)  const {

    if (x < d->_xLeft)      return false;
    if (x > d->_xRight)     return false;
    if (y < d->_yTop)       return false;
    if (y > d->_yBottom)    return false;

    return true;
}

void Document::changeMinimalSize(qreal width, qreal height) {
    if (width>=0) d->_minWidth = width;
    if (height>=0) d->_minHeight = height;

    if (width > d->_xRight - d->_xLeft) {
        d->_xLeft -= (d->_xRight- d->_xLeft - width)/2;
        d->_xRight += (d->_xRight- d->_xLeft - width)/2;
        emit resized();
    }
    else {
        resizeDocumentBorder(BorderLeft);
    }

    if (height > d->_yBottom - d->_yTop) {
        d->_yTop -= (d->_yBottom - d->_yTop - height)/2;
        d->_yBottom += (d->_yBottom - d->_yTop - height)/2;
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
        foreach( Data* n,  dataStructures().at(i)->dataList() ){
            if (n->x() < d->_xLeft+GraphScene::kBORDER) {
                setXLeft(d->_xLeft-GraphScene::kBORDER);
                emit resized();
            }
            if (n->x() > d->_xRight-GraphScene::kBORDER) {
                setXRight(d->_xRight+GraphScene::kBORDER);
                emit resized();
            }
            if (n->y() < d->_yTop+GraphScene::kBORDER) {
                setYTop(d->_yTop-GraphScene::kBORDER);
                emit resized();
            }
            if (n->y() > d->_yBottom-GraphScene::kBORDER) {
                setYBottom(d->_yBottom+GraphScene::kBORDER);
                emit resized();
            }
        }
    }
}

void Document::resizeDocumentBorder(Document::Border orientation) {
    bool empty = true;
    int elements = dataStructures().size();

    // scans doubled border of specified size: if empty or not
    for (int i = 0; i < elements; i++) {
    foreach( Data* n,  dataStructures().at(i)->dataList() ){
        switch (orientation) {
        case BorderLeft: {
            if (n!=0 && n->x() < d->_xLeft+GraphScene::kBORDER*2) empty=false;
            break;
        }
        case BorderRight: {
            if (n!=0 && n->x() > d->_xRight-GraphScene::kBORDER*2) empty=false;
            break;
        }
        case BorderTop: {
            if (n!=0 && n->y() < d->_yTop+GraphScene::kBORDER*2) empty=false;
            break;
        }
        case BorderBottom: {
            if (n!=0 && n->y() > d->_yBottom-GraphScene::kBORDER*2) empty=false;
            break;
        }
        }
    }
    }

    //TODO connect to graphvisualeditor-size
    if (empty==true) {
        switch (orientation) {
        case BorderLeft: {
            if (d->_xRight-d->_xLeft < GraphScene::kBORDER*4)
                return;
            setXLeft(d->_xLeft+GraphScene::kBORDER);
            emit resized();
            break;
        }
        case BorderRight: {
            if (d->_xRight-d->_xLeft < GraphScene::kBORDER*4)
                return;
            setXRight(d->_xRight-GraphScene::kBORDER);
            emit resized();
            break;
        }
        case BorderTop: {
            if (d->_yBottom-d->_yTop < GraphScene::kBORDER*4)
                return;
            setYTop(d->_yTop+GraphScene::kBORDER);
            emit resized();
            break;
        }
        case BorderBottom: {
            if (d->_yBottom-d->_yTop < GraphScene::kBORDER*4)
                return;
            setYBottom(d->_yBottom-GraphScene::kBORDER);
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
  foreach (DataStructure * ds, d->_dataStructures)
    ds->cleanUpBeforeConvert();
}


void Document::setActiveDataStructure(DataStructure *g){
    if (d->_dataStructures.indexOf(g) != -1){
        d->_activeDataStructure = g;
        emit activeDataStructureChanged(g);
        d->_modified = true;
    }
}

DataStructure* Document::addDataStructure(QString name) {
    DataStructure *g = DataStructurePluginManager::self()->createNewDataStructure(this,
                                                                                  d->_dataStructureType->name());
    g->setName(name);
    d->_dataStructures.append(g);
    d->_activeDataStructure = g;
    d->_modified = true;
    connect(g, SIGNAL(changed()), this, SLOT(setModified()));
    emit dataStructureCreated(g);
    return g;
}

void Document::savedDocumentAt(const QString& fileName) {
    d->_lastSavedDocumentPath = fileName;
}

const QString& Document::documentPath() const {
    return d->_lastSavedDocumentPath;
}

void Document::remove(DataStructure *dataStructure){
    d->_dataStructures.removeOne(dataStructure);
    d->_modified = true;
}

bool Document::saveAsInternalFormat(const QString& filename) {
    d->_buf.clear();

    KSaveFile saveFile( !filename.endsWith(".graph") ? QString("%1.graph").arg(filename) : filename);

    if (!saveFile.open()) {
        kDebug() << "Error: File Not Open";
        return false;
    }

    QTextStream stream(&saveFile);
    stream.setCodec("UTF-8");

    for (int i = 0; i < d->_dataStructures.count(); i++) {
        DataStructure *g = d->_dataStructures.at(i);

        d->_buf += QString("[DataStructure %1] \n").arg(i).toUtf8();

        savePropertiesInternalFormat(g);

        foreach( Data *n, g->dataList()) {
            d->_buf += QString("[Data %1]\n").arg(g->dataList().indexOf(n)).toUtf8();
            savePropertiesInternalFormat(n);
        }

        int from, to;
        foreach( Pointer *e, g->pointers()) {
            from = g->dataList().indexOf(e->from());
            to = g->dataList().indexOf(e->to());
            d->_buf += QString("[Pointer %1->%2]\n").arg(from).arg(to).toUtf8();
            savePropertiesInternalFormat(e);
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
        kDebug() << "File not open " << filename.toUtf8();
        return;
    }

    DataStructure* tmpDataStructure = 0;
   //DataStructureGroup *tmpGroup = 0;
    QObject *tmpObject = 0;


    QTextStream in(&f);
    in.setCodec("UTF-8");

    while (!in.atEnd()) {
        QString str = in.readLine().simplified();

        if (str.startsWith('#')) { //! Ignore it, commented line.
            continue;
        }

        else if (str.startsWith("[DataStructure")) {
            QString gName = str.section(' ',1,1);
            gName.remove(']');
            tmpDataStructure = DataStructurePluginManager::self()->createNewDataStructure(this);
            tmpDataStructure->setName(gName.toAscii());
            tmpObject = tmpDataStructure;
            d->_dataStructures.append(tmpDataStructure);
            kDebug() << "DataStructure Created";
        }

        else if (str.startsWith("[Data")) {
            QString nName = str.section(' ',1,1);
            nName.remove(']');
            tmpObject = tmpDataStructure->addData(nName);
            kDebug() << "Data Created";
        }

        else if (str.startsWith("[Pointer")) {
            QString eName = str.section(' ',1,1);
            eName.remove(']');

            QString nameFrom = eName.section("->", 0,0);
            QString nameTo = eName.section("->", 1,1);

            tmpObject = tmpDataStructure->addPointer(tmpDataStructure->dataList().at(nameFrom.toInt()),
                                                     tmpDataStructure->dataList().at(nameTo.toInt()));
            kDebug() << "Pointer Created";
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
        else {
//            // tmpGroup->append( tmpDataStructure->data(str));
        }
    }
    d->_modified = false;
    kDebug() << "DataStructure Document Loaded.";
}

DataStructure *Document::activeDataStructure() const { return d->_activeDataStructure; }

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
