/*
 *  SPDX-FileCopyrightText: 2013-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#ifndef METHOD_H
#define METHOD_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QVariantList>

class Parameter;

class Method : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QVariantList description READ description)
    Q_PROPERTY(QVariant parameters READ parametersVar)
    Q_PROPERTY(QString returnType READ returnType)
    Q_PROPERTY(QString returnTypeLink READ returnTypeLink)
    Q_PROPERTY(QString documentAnchor READ apiDocumentAnchor)

public:
    explicit Method(QObject *parent = nullptr);

    QString name() const;
    void setName(const QString &name);
    QVariantList description() const;
    void setDescription(const QStringList &description);
    QString returnType() const;
    void setReturnType(const QString &type);
    QVariant parametersVar() const;
    QList<Parameter*> parameters() const;
    void addParameter(const QString &name, const QString &type, const QString &info, const QString &typeLink);
    QString apiDocumentAnchor();
    QString returnTypeLink() const;
    void setReturnTypeLink(const QString &link);

private:
    Q_DISABLE_COPY(Method)
    QString m_name;
    QStringList m_description;
    QString m_returnType;
    QString m_returnTypeLink;
    QList<Parameter*> m_parameters;
};

// Q_DECLARE_METATYPE(QList<ParameterDocumentation*>);
// Q_DECLARE_METATYPE(ParameterDocumentation*);

#endif
