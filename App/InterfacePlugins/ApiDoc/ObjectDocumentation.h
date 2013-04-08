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

#ifndef OBJECT_DOCUMENTATION_H
#define OBJECT_DOCUMENTATION_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <KUrl>

class PropertyDocumentation;
class MethodDocumentation;

class ObjectDocumentation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString syntaxExample READ syntaxExample)
    Q_PROPERTY(QVariantList description READ description)

public:
    explicit ObjectDocumentation(QObject *parent = 0);
    ~ObjectDocumentation();

    QString id() const;
    void setId(const QString& id);
    QString title() const;
    void setTitle(const QString &title);
    QVariantList description() const;
    void setDescription(const QStringList &description);
    QString syntaxExample() const;
    void setSyntaxExample(const QString &syntaxExample);
    QList<PropertyDocumentation *> properties() const;
    void addProperty(PropertyDocumentation *property);
    QList<MethodDocumentation *> methods() const;
    void addMethod(MethodDocumentation *method);
    QString objectParent() const;
    void setObjectParent(const QString &id);
    QString apiDocumentIdentifier();
    QString apiDocumentAnchor();

private:
    Q_DISABLE_COPY(ObjectDocumentation)
    QString _title;
    QString _id;
    QString _objectParent;
    QStringList _description;
    QString _syntaxExample;
    QList< PropertyDocumentation *> _properties;
    QList< MethodDocumentation *> _methods;
};

#endif // OBJECT_DOCUMENTATION_H
