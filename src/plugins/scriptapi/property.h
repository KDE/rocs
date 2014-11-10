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

#ifndef PROPERTY_H
#define PROPERTY_H

#include <QObject>
#include <QList>
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
    explicit Property(QObject *parent = 0);

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
