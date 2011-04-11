/* This file is part of Rocs,
   Copyright (C) 2008 by:
   Tomaz Canabrava <tomaz.canabrava@gmail.com>
   Ugo Sangiori <ugorox@gmail.com>

   Rocs is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Rocs is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Rocs; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QList>
#include <QPointF>
#include <QObject>
#include "rocslib_export.h"
#include <QSize>

class DataStructurePluginInterface;
class DataStructure;
class QtScriptBackend;
class DocumentPrivate;

class ROCSLIB_EXPORT Document : public QObject
{
    Q_OBJECT
    Q_PROPERTY(    qreal xLeft READ xLeft WRITE setXLeft)
    Q_PROPERTY(    qreal xRight READ xRight WRITE setXRight)
    Q_PROPERTY(    qreal yTop READ yTop WRITE setYTop)
    Q_PROPERTY(    qreal yBottom READ yBottom WRITE setYBottom)

public:
    enum Type {Simple = 0, Oriented};
    enum Border {BorderLeft, BorderBottom, BorderRight, BorderTop};

    Document(   const QString& name,
                qreal xLeft=-200,
                qreal xRight=200,
                qreal yTop=-200,
                qreal yBottom=200,
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
    QList<DataStructure*>& dataStructures() const;

    void remove(DataStructure *dataStructure);
    QSizeF size();

    /** @brief clear data that only is usefull for a type of data structure and that cannot be converted to others from all data structeres of this document.
    */
    virtual void cleanUpBeforeConvert();

    /** @brief return the name of data structure used to build this document. */
    QString dataStructureTypeName()const;

    DataStructurePluginInterface * dataStructurePlugin() const;

public slots:
    void setModified(const bool mod = true);
    DataStructure *addDataStructure(QString name = "untitled");
    DataStructure *activeDataStructure() const ;
    void setActiveDataStructure(DataStructure *g);

    void setXLeft(qreal xLeftValue);
    void setXRight(qreal xRightValue);
    void setYTop(qreal yTopValue);
    void setYBottom(qreal yButtomValue);

    qreal xLeft() const;
    qreal yTop() const;
    qreal xRight() const;
    qreal yBottom() const;

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
    void dataStructureCreated( DataStructure *g);
    void dataStructureRemoved(int i);
    void nameChanged(QString name);
    void heightChanged(qreal height);
    void widthChanged(qreal width);
    void activeDataStructureChanged(DataStructure* g);
    void resized();

private:
    DocumentPrivate *d;
    void  savePropertiesInternalFormat(QObject *o);
};

#endif
