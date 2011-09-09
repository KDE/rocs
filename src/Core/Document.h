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

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QList>
#include <QPointF>
#include <QObject>
#include "rocslib_export.h"
#include "Rocs_Typedefs.h"
#include <QSize>
#include <QRectF>
#include <boost/shared_ptr.hpp>

class DataStructurePluginInterface;
class DataStructure;
class QtScriptBackend;
class DocumentPrivate;

class ROCSLIB_EXPORT Document : public QObject
{
    Q_OBJECT
    Q_PROPERTY(    qreal left READ left WRITE setLeft)
    Q_PROPERTY(    qreal right READ right WRITE setRight)
    Q_PROPERTY(    qreal top READ top WRITE setTop)
    Q_PROPERTY(    qreal bottom READ bottom WRITE setBottom)

public:
    enum Type {Simple = 0, Oriented};
    enum Border {BorderLeft, BorderBottom, BorderRight, BorderTop};

    Document(   const QString& name,
                qreal left=-200,
                qreal right=200,
                qreal top=-200,
                qreal bottom=200,
                QObject *parent = 0
            );
    Document(const Document& gd);
    ~Document();

    bool isModified() const;

    void setName(const QString& name);
    QString name() const;

    void savedDocumentAt(const QString& fileName);
    const QString& documentPath() const;
    bool saveAsInternalFormat(const QString& filename);
    void loadFromInternalFormat(const QString& filename);

    QtScriptBackend * engineBackend() const;
    QList< DataStructurePtr >& dataStructures() const;

    void remove(DataStructurePtr dataStructure);
    /** return the size of document' (visual) area
     */
    QRectF size();

    /** @brief clear data that only is usefull for a type of data structure and that cannot be converted to others from all data structeres of this document.
    */
    virtual void cleanUpBeforeConvert();

    /** @brief return the name of data structure used to build this document. */
    QString dataStructureTypeName()const;

    DataStructurePluginInterface * dataStructurePlugin() const;

public slots:
    void setModified(const bool mod = true);
    DataStructurePtr addDataStructure(QString name = "untitled");
    DataStructurePtr activeDataStructure() const ;
    void setActiveDataStructure(DataStructurePtr g);

    void setLeft(qreal leftValue);
    void setRight(qreal rightValue);
    void setTop(qreal topValue);
    void setBottom(qreal buttomValue);

    qreal left() const;
    qreal top() const;
    qreal right() const;
    qreal bottom() const;

    qreal height() const;
    qreal width() const;

    /**
     * estimates by looking at all node positions if a resize of the document is necessary.
     */
    void resizeDocumentIncrease();
    void resizeDocumentBorder(Document::Border orientation);
    void changeMinimalSize(qreal width, qreal height);

    /**
     * tests if given point is containted at document layer
     */
    bool isPointAtDocument(qreal x, qreal y) const;
    bool isPointAtDocument(QPointF point) const;

signals:
    void dataStructureCreated(DataStructurePtr g);
    void dataStructureRemoved(int i);
    void nameChanged(QString name);
    void heightChanged(qreal height);
    void widthChanged(qreal width);
    void activeDataStructureChanged(DataStructurePtr g);
    void resized();

private:
    boost::shared_ptr<DocumentPrivate> d;
    void  savePropertiesInternalFormat(QObject *o);
};

#endif
