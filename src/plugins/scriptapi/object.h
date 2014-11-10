/*
 *  Copyright 2013-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License or (at your option) version 3 or any later version
 *  accepted by the membership of KDE e.V. (or its successor approved
 *  by the membership of KDE e.V.), which shall act as a proxy
 *  defined in Section 14 of version 3 of the license.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <QObject>
#include <QList>
#include <QStringList>

class Property;
class Method;

class Object : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString syntaxExample READ syntaxExample)
    Q_PROPERTY(QVariantList description READ description)

public:
    enum ComponentType {
        Edge,
        Node,
        KernelModule
    };

    explicit Object(QObject *parent = 0);
    ~Object();

    QString id() const;
    void setId(const QString& id);
    ComponentType componentType() const;
    void setComponentType(const QString &compenentTypeName);
    QString title() const;
    void setTitle(const QString &title);
    QVariantList description() const;
    void setDescription(const QStringList &description);
    QString syntaxExample() const;
    void setSyntaxExample(const QString &syntaxExample);
    QList<Property*> properties() const;
    void addProperty(Property *property);
    QList<Method*> methods() const;
    void addMethod(Method *method);
    QString objectParent() const;
    void setObjectParent(const QString &id);
    QString apiDocumentIdentifier();
    QString apiDocumentAnchor();

private:
    Q_DISABLE_COPY(Object)
    QString m_title;
    QString m_id;
    ComponentType m_componentType;
    QString m_objectParent;
    QStringList m_description;
    QString m_syntaxExample;
    QList<Property*> m_properties;
    QList<Method*> m_methods;
};

#endif
