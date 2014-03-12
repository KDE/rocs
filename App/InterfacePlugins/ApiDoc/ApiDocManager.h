/*
    This file is part of Rocs.
    Copyright 2013  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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

#ifndef APIDOCMANAGER_H
#define APIDOCMANAGER_H

#include <QObject>
#include <QMap>
#include <QHash>

class ObjectDocumentation;
class QUrl;
class QDomDocument;
class QFile;
class QXmlSchema;

/**
 * \class ApiDocManager
 *
 * Load and provide api documentation objects.
 */
class ApiDocManager : public QObject
{
    Q_OBJECT

public:
    explicit ApiDocManager(QObject *parent = 0);

    /**
     * This method loads all api documentation files.
     */
    void loadLocalData();

    /**
     * \return list of all loaded language specifications
     */
    QList<ObjectDocumentation *> objectApiList() const;

    ObjectDocumentation * objectApi(int index) const;

    /**
     * Generates HTML document with the API documentation for the specified object.
     * The generated object is cached such that only the first call to this method
     * is expensive, later calls only return the cached page;
     *
     * \param identifier is the identifier of an ObjectDocumentation object
     * \return HTML content
     */
    QString objectApiDocument(const QString &identifier);

    /**
     * Generates HTML document with the an overview over all available API objects.
     *
     * \return HTML content
     */
    QString apiOverviewDocument() const;

    /**
     * Load documenation object specification from locally stored XML file.
     *
     * \param path is the local XML file containing the object API specification
     * \return true if loaded successfully, otherwise false
     */
    bool loadObjectApi(const QUrl &path);

signals:
    void objectApiAdded();
    void objectApiAboutToBeAdded(ObjectDocumentation*,int);
    void objectApiRemoved();
    void objectApiAboutToBeRemoved(int,int);

private:
    /**
     * Load XSD file given by its file name (without ".xsd" suffix). The method searches exclusively
     * the standard install dir for XSD files in subdirectory "schemes/".
     *
     * \param schemeName name of the Xml schema without suffix
     * \return loaded XML Schema
     */
    QXmlSchema loadXmlSchema(const QString &schemeName) const;

    /**
     * Load XML file given by \p file that confirms with XML schema \p scheme.
     *
     * \param path is the path to the XML file to be loaded
     * \param scheme is the XML schema describing the DOM
     * \return the loaded DOM document
     */
    QDomDocument loadDomDocument(const QUrl &path, const QXmlSchema &schema) const;

    QList<ObjectDocumentation *> _objectApiList;
    QHash<QString, QString> _objectApiDocuments;
    QList<QString> _objectApiCache; // list of all objects available
};

#endif // APIDOCMANAGER_H
