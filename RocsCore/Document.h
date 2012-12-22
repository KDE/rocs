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

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "RocsCoreExport.h"
#include "CoreTypes.h"
#include <QList>
#include <QObject>
#include <QSize>
#include <QRectF>
#include <QMap>

class DocumentPrivate;
class KUrl;
class DataStructurePluginInterface;
class DataStructure;
class QtScriptBackend;
class QSvgRenderer;
class QPointF;

class ROCSLIB_EXPORT Document : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal left READ left WRITE setLeft)
    Q_PROPERTY(qreal right READ right WRITE setRight)
    Q_PROPERTY(qreal top READ top WRITE setTop)
    Q_PROPERTY(qreal bottom READ bottom WRITE setBottom)

public:
    enum Border {BorderLeft, BorderBottom, BorderRight, BorderTop};

    explicit Document(const QString &name,
             qreal left = -200,
             qreal right = 200,
             qreal top = -200,
             qreal bottom = 200,
             QObject *parent = 0
            );
    ~Document();

    bool isModified() const;

    void setName(const QString &name);
    QString name() const;

    /**
     * Save graph document in former file.
     * It is expected that \see fileUrl() is not empty.
     */
    void save();

    /**
     * Save graph document under the given \p fileUrl. Old file is not changed.
     *
     * \param fileUrl path to local file to that document shall be saved
     */
    void saveAs(const QString& fileUrl);

    /**
     * \return path used for saving
     */
    QString fileUrl() const;

    /**
     * Set file path used for saving.
     *
     * \param fileUrl path to local file for saving the document
     */
    void setFileUrl(const KUrl &fileUrl);

    /**
     * Evaluates given string and returns true if identifier is valid, otherwise returns false.
     *
     * \param identifier the string to be tested if it is valid property identifier
     */
    static bool isValidIdentifier(const QString &identifier);

    QtScriptBackend * engineBackend() const;
    QList<DataStructurePtr>& dataStructures() const;

    void remove(DataStructurePtr dataStructure);

    /**
     * Remove everything contained at this document.
     */
    void clear();

    /**
     * \return the size of document' (visual) area
     */
    QRectF size();

    /** @brief clear data that only is useful for a type of data structure and that cannot be converted to others from all data structeres of this document.
     * TODO this method should be protected
     */
    virtual void cleanUpBeforeConvert();

    /**
     * \return backend of this document
     */
    DataStructurePluginInterface * backend() const;

    /**
     * Set data structure plugin for this document. This function is only safe if the document
     * is empty, otherwise behavior is undefined.
     */
    void setBackend(const QString &pluginIdentifier);

    /**
     * Change data structure backend of this document to currently active backend.
     * \see DataStructureBackend::activeBackend(). Changing the backend of a data structure is
     * a LOSSY operation (converting between backends and converting back will result in data loss!)
     */
    void changeBackend();

    /** Register new type for data elements. If identifier is alreade in use or if no identifier is
     * provided, a new identifier is created.
     *
     * \param name of the dataType
     * \param identifier is optional identifier for data type
     * \return positive integer >0 if successfully registered, else <=0
     */
    int registerDataType(const QString &name, int identifier = 0);

    /** Register new type for pointers. If identifier is already in use or if no identifier is
     * provided, a new identifier is created.
     *
     * \param name of the pointerType
     * \return positive integer >0 if successfully registered, else <=0
     */
    int registerPointerType(const QString &name, int identifier = 0);

    /** removes this data type and all data elements of this type
     *
     * \param dataType is positive id>0
     * \return true if a dataType was removed
     */
    bool removeDataType(int dataType);

    /** Removes this pointer type and all data elements of this type.
     * Aborts and returns "false" if pointer type is "0" or if the pointertype does not exists.
     *
     * \param pointerType is positive id>0
     * \return true if a dataType was removed
     */
    bool removePointerType(int pointerType);

    PointerTypePtr pointerType(int pointerType) const;
    DataTypePtr dataType(int dataType) const;

    /**
     * Returns data type for element groups. If type is not registered yet, it will be created.
     */
    int groupType();

    /**
     * Getter for all registered data types.
     * \return list of all data type ids
     */
    QList<int> dataTypeList() const;

    /**
     * Getter for all registered pointer types.
     * \return list of all pointer type ids
     */
    QList<int> pointerTypeList() const;

    QString iconPackage() const;

    qreal left() const;
    qreal top() const;
    qreal right() const;
    qreal bottom() const;

    qreal height() const;
    qreal width() const;

    /**
     * tests if given point is containted at document layer
     */
    bool isPointAtDocument(qreal x, qreal y) const;
    bool isPointAtDocument(const QPointF &point) const;

public Q_SLOTS:
    void setModified(const bool mod = true);

    /**
     * estimates by looking at all node positions if a resize of the document is necessary.
     */
    void resizeDocumentIncrease();
    void resizeDocumentBorder(Document::Border orientation);
    void changeMinimalSize(qreal width, qreal height);

    /**
     * Add data structure to graph document with name \p name.
     * If no name is given, the name of the used data structure is used.
     *
     * \param name is the optional name of the to be created data structure
     * \return the added data structure
     */
    DataStructurePtr addDataStructure(const QString &name = QString());

    /**
     * Add data structure \p dataStructure to the document.
     *
     * \param dataStructure the data structure to be added to the document
     * \return the added data structure for argument chaining
     */
    DataStructurePtr addDataStructure(DataStructurePtr dataStructure);

    /**
     * \return the currently selected data structure, if none exists the shared pointer is invalid
     */
    DataStructurePtr activeDataStructure() const;

    /**
     * Sets the active data structure of graph document with index \p index in the
     * data structure list. To get list of indices \see dataStructures();
     *
     * \param index is the index of the data structure in internal data structure list
     */
    void setActiveDataStructure(int index);
    void setActiveDataStructure(DataStructurePtr g);

    void setLeft(qreal leftValue);
    void setRight(qreal rightValue);
    void setTop(qreal topValue);
    void setBottom(qreal buttomValue);

    void updateGraphics(DataTypePtr dataType);
    void updateGraphics(PointerTypePtr pointerType);

Q_SIGNALS:
    void dataStructureCreated(DataStructurePtr g);
    void dataStructureListChanged();
    void dataTypeCreated(int identifier);
    void pointerTypeCreated(int identifier);
    void dataTypeRemoved(int identifier);
    void pointerTypeRemoved(int identifier);
    void nameChanged(QString name);
    void heightChanged(qreal height);
    void widthChanged(qreal width);
    void activeDataStructureChanged(DataStructurePtr g);
    void resized();

private:
    // d-pointer
    const QScopedPointer<DocumentPrivate> d;
    Document(const Document&);
    Document& operator=(const Document&);
};

#endif
