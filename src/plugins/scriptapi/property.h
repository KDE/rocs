/*
 *  SPDX-FileCopyrightText: 2013-2014 Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#ifndef PROPERTY_H
#define PROPERTY_H

#include <QList>
#include <QObject>
#include <QStringList>

class Property : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QVariantList description READ description)
    Q_PROPERTY(QString type READ type)
    Q_PROPERTY(QString typeLink READ typeLink)
    Q_PROPERTY(QString documentAnchor READ apiDocumentAnchor)

public:
    explicit Property(QObject *parent = nullptr);

    QString name() const;
    void setName(const QString &name);
    /**
     * \return list of paragraphs
     */
    QVariantList description() const;
    void setDescription(const QStringList &description);
    QString type() const;
    void setType(const QString &type);
    QString typeLink() const;
    void setTypeLink(const QString &link);
    QString apiDocumentAnchor();

private:
    Q_DISABLE_COPY(Property)
    QString m_name;
    QStringList m_description;
    QString m_type;
    QString m_typeLink;
};

#endif
