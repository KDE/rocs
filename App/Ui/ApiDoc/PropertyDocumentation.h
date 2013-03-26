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

#ifndef PROPERTY_DOCUMENTATION_H
#define PROPERTY_DOCUMENTATION_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <KUrl>


class PropertyDocumentation : public QObject
{
    Q_OBJECT

public:
    explicit PropertyDocumentation(QObject *parent = 0);

    QString name() const;
    void setName(const QString &name);
    /**
     * \return list of paragraphs
     */
    QStringList description() const;
    void setDescription(const QStringList &description);
    QString type() const;
    void setType(const QString &type);

private:
    Q_DISABLE_COPY(PropertyDocumentation)
    QString _name;
    QStringList _description;
    QString _type;
};

#endif // PROPERTY_DOCUMENTATION_H
