/*
 *  SPDX-FileCopyrightText: 2013-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
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
        Document,
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
